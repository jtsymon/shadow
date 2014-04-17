
#include "buffered.h"
#include "shaders.h"

#define buffer_size 1024
static GLfloat  buffer[buffer_size * 2];
static int      p;
static GLenum   buffer_mode = GL_TRIANGLES;
static RGBA     buffer_colour = { 255, 255, 255, 255 };
static GLuint   buffer_shader;

static GLuint   vertex_array;
static GLuint   vertex_buffer;

#define buffer_default_shader RENDER.shaders.colour

int init_buffer() {
    glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
    
    glGenBuffers(1, &vertex_buffer);
    
    buffer_shader = buffer_default_shader;
    
    return 0;
}

void buffer_reset() {
    p = 0;
}

/**
 * Draws the buffered items with a shader
 */
void buffer_end() {
    if(p == 0) return;
    
    glUseProgram(buffer_shader);
    
    glBindVertexArray(vertex_array);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, p * 2 * sizeof(GLfloat), buffer, GL_STATIC_DRAW);
    
    glUniform4f(glGetUniformLocation(buffer_shader, "colour_in"),
            (float)buffer_colour.r / 255, (float)buffer_colour.g / 255,
            (float)buffer_colour.b / 255, (float)buffer_colour.a / 255);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(
                    0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    2, // size
                    GL_FLOAT, // type
                    GL_FALSE, // normalized?
                    0, // stride
                    (void*) 0 // array buffer offset
                    );
    glDrawArrays(buffer_mode, 0, p);
    glDisableVertexAttribArray(0);
    
    buffer_reset();
}

/**
 * Changes the drawing mode
 */
void buffer_set_mode(GLenum mode) {
    if(mode == buffer_mode) return;
    buffer_end();
    buffer_mode = mode;
}

/**
 * Changes the colour
 */
void buffer_set_colour(RGBA colour) {
    if(colour.r == buffer_colour.r && colour.g == buffer_colour.g &&
            colour.b == buffer_colour.b && colour.a == buffer_colour.a) return;
    buffer_end();
    buffer_colour = colour;
}

/**
 * Changes the shader
 */
void buffer_set_shader(GLuint shader) {
    if(!shader) shader = buffer_default_shader;
    if(shader == buffer_shader) return;
    buffer_end();
    buffer_shader = shader;
}

int buffer_check_space() {
    return buffer_size - p;
}

/**
 * Adds a single point
 */
int buffer_add(GLfloat x, GLfloat y) {
    if(buffer_size - p <= 0) {
        buffer_end(0);
    }
    buffer[p << 1] = x; buffer[(p << 1) + 1] = y;
    return buffer_size - ++p;
}

/**
 * Adds some points
 */
int buffer_add_n(int n, GLfloat points[]) {
    if(buffer_size - p <= n - 1) {
        buffer_end(0);
    }
    int i;
    for(i = 0; i < n; i++) {
        buffer[p << 1] = points[i << 1]; buffer[(p << 1) + 1] = points[(i << 1) + 1];
        p++;
    }
    return buffer_size - p;
}

#undef buffer_size