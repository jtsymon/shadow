#include "Graphics.h"

#include "../misc.h"
#include "Colours.h"
#include "BitmapFont.h"
#include "Shaders.h"
#include "../main.h"

GLuint Graphics::vertex_buffer[n_buffers];
GLuint Graphics::vertex_array[n_buffers];
GLuint Graphics::shaders[n_shaders];
int Graphics::width;
int Graphics::height;
float Graphics::two_over_width;
float Graphics::two_over_height;

Graphics::Graphics(int width, int height) {
  // setup GL

  Graphics::update_dimensions(width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Enable smooth shading
  glShadeModel(GL_SMOOTH);

  //    glEnable( GL_LINE_SMOOTH );
  //    glEnable( GL_POLYGON_SMOOTH );
  //    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  //    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

  // 2d
  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (int i = 0; i < n_buffers; i++) {
    glGenVertexArrays(1, &Graphics::vertex_array[i]);
    glBindVertexArray(Graphics::vertex_array[i]);

    glGenBuffers(1, &Graphics::vertex_buffer[i]);
  }

  // init shaders
  Graphics::shaders[GRAPHICS_COLOUR_SHADER] = compile_program("shaders/pass_through.vert", "shaders/colour.frag");
  Graphics::shaders[GRAPHICS_SHADOW_SHADER] = compile_program("shaders/pass_through.vert", "shaders/shadow_mask.frag");
  Graphics::shaders[GRAPHICS_TEXTURE_SHADER] = compile_program("shaders/texture.vert", "shaders/texture.frag");
  Graphics::shaders[GRAPHICS_BLUR_SHADER] = compile_program("shaders/texture.vert", "shaders/blur.frag");
}

Graphics& Graphics::get(int width, int height) {
  static Graphics instance(width, height);
  return instance;
}

void Graphics::update_dimensions(int width, int height) {
  Graphics::width = width;
  Graphics::height = height;
  Graphics::two_over_width = 2.f / Graphics::width;
  Graphics::two_over_height = 2.f / Graphics::height;

  glViewport(0, 0, Graphics::width, Graphics::height);
}

float Graphics::window_to_gl_x(int x) {
  return (float) x * Graphics::two_over_width - 1.f;
}

float Graphics::window_to_gl_y(int y) {
  return 1.f - (float) y * Graphics::two_over_height;
}

float Graphics::game_to_gl_x(int x) {
  return (float) x / MAP_SCALE * Graphics::two_over_width - 1.f;
}

float Graphics::game_to_gl_y(int y) {
  return 1.f - (float) y / MAP_SCALE * Graphics::two_over_height;
}

/**
 * Draws a number of points
 * TODO: Add some way of handling shader parameters
 * (currently this can't properly handle different shaders if have different parameters)
 * @param colour
 * @param mode
 * @param n
 * @param points
 */
void Graphics::draw(RGBA colour, GLuint mode, GLuint shader, int n, GLfloat *points) {

  glUseProgram(shader);

  glBindVertexArray(Graphics::vertex_array[0]);

  glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, n * 2 * sizeof (GLfloat), points, GL_STATIC_DRAW);

  glUniform4f(glGetUniformLocation(shader, "colour_in"),
              (float) colour.r / 255, (float) colour.g / 255,
              (float) colour.b / 255, (float) colour.a / 255);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
  glVertexAttribPointer(
                        0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
                        2, // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0, // stride
                        (void*) 0 // array buffer offset
                        );
  glDrawArrays(mode, 0, n);

  glDisableVertexAttribArray(0);
}

void Graphics::draw(Batch &batch) {
  this->draw(batch.colour, batch.mode, batch.shader, batch.p, batch.buffer);
  batch.clear();
}

void Graphics::fill_triangle(RGBA colour, float x1, float y1, float x2, float y2, float x3, float y3) {

  GLfloat points[] = {
    x1, y1,
    x2, y2,
    x3, y3
  };

  this->draw(colour, GL_TRIANGLES, this->shaders[GRAPHICS_COLOUR_SHADER], 3, points);
}

void Graphics::fill_rectangle(RGBA colour, float x1, float y1, float x2, float y2) {

  GLfloat points[] = {
    x1, y1,
    x2, y1,
    x2, y2,
    x1, y1,
    x1, y2,
    x2, y2
  };

  this->draw(colour, GL_TRIANGLES, this->shaders[GRAPHICS_COLOUR_SHADER], 6, points);
}

void Graphics::fill_rectangle(RGBA colour, Layout layout) {
  this->fill_rectangle(colour, Graphics::window_to_gl_x(layout.getLeft()), Graphics::window_to_gl_y(layout.getTop()),
                       Graphics::window_to_gl_x(layout.getRight()), Graphics::window_to_gl_y(layout.getBottom()));
}

int Batch::check_space() {
  return this->size - this->p;
}

int Batch::add(GLfloat x, GLfloat y) {
  int remaining = this->size - (this->p + 1);
  if (remaining < 0) {
    throw Exception("Batch out of space!");
  }
  this->buffer[this->p << 1] = x;
  this->buffer[(this->p << 1) + 1] = y;
  this->p++;
  return remaining;
}

int Batch::add(int n, const GLfloat points[]) {
  int remaining = this->size - (this->p + n);
  if (remaining < 0) {
    throw Exception("Batch out of space!");
  }
  int i;
  for (i = 0; i < n; i++) {
    this->buffer[this->p << 1] = points[i << 1];
    this->buffer[(this->p << 1) + 1] = points[(i << 1) + 1];
    this->p++;
  }
  return remaining;
}

void Batch::clear() {
  this->p = 0;
}
