
#include "Graphics.h"
#include "../misc.h"
#include "Colours.h"
#include "BitmapFont.h"
#include "Shaders.h"
#include "../main.h"

GLuint Graphics::vertex_buffer[n_buffers];
GLuint Graphics::vertex_array[n_buffers];
GLuint Graphics::shaders[n_shaders];

Graphics::Graphics() {
    // init shaders
    Graphics::shaders[GRAPHICS_COLOUR_SHADER] = compile_program("shaders/pass_through.vert", "shaders/colour.frag");
    Graphics::shaders[GRAPHICS_SHADOW_SHADER] = compile_program("shaders/pass_through.vert", "shaders/shadow_mask.frag");
    Graphics::shaders[GRAPHICS_TEXTURE_SHADER] = compile_program("shaders/texture.vert", "shaders/texture.frag");
    Graphics::shaders[GRAPHICS_BLUR_SHADER] = compile_program("shaders/texture.vert", "shaders/blur.frag");
}

Graphics& Graphics::get() {
    static Graphics instance;
    return instance;
}

static float two_over_width, two_over_height;

void update_dimensions() {
    two_over_width = 2.f / width;
    two_over_height = 2.f / height;

    printf("2/w=%f, 2/h=%f\n", two_over_width, two_over_height);
}

float game_to_gl_x(int x) {
    return (float) x * two_over_width - 1;
}

float game_to_gl_y(int y) {
    return 1 - (float) y * two_over_height;
}

int init_vertex_buffers() {
    int i;
    for (i = 0; i < n_buffers; i++) {
        glGenVertexArrays(1, &Graphics::vertex_array[i]);
        glBindVertexArray(Graphics::vertex_array[i]);

        glGenBuffers(1, &Graphics::vertex_buffer[i]);
    }

    return 0;
}

/**
 *  General OpenGL initialization function
 */
int initGL() {

    glViewport(0, 0, width, height);

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

    update_dimensions();

    return init_vertex_buffers();
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
}

void Graphics::fill_triangle(RGBA colour, int x1, int y1, int x2, int y2, int x3, int y3) {
    
    GLfloat points[] = {
        game_to_gl_x(x1), game_to_gl_y(y1),
        game_to_gl_x(x2), game_to_gl_y(y2),
        game_to_gl_x(x3), game_to_gl_y(y3)
    };
    
    this->draw(colour, GL_TRIANGLES, this->shaders[GRAPHICS_COLOUR_SHADER], 3, points);
}

void Graphics::fill_rectangle(RGBA colour, int x1, int y1, int x2, int y2) {
    float xa = game_to_gl_x(x1), ya = game_to_gl_y(y1),
            xb = game_to_gl_x(x2), yb = game_to_gl_y(y2);

    GLfloat points[] = {
        xa, ya,
        xb, ya,
        xb, yb,
        xa, ya,
        xa, yb,
        xb, yb
    };
    
    this->draw(colour, GL_TRIANGLES, this->shaders[GRAPHICS_COLOUR_SHADER], 6, points);
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