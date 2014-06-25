/* 
 * File:   graphics.h
 * Author: jts
 *
 * Created on 12 January 2014, 8:06 PM
 */

#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include <GL/glew.h>
#include "Colours.h"

#define n_buffers 2
#define render_buffer_size 1024

#define GRAPHICS_COLOUR_SHADER 0
#define GRAPHICS_SHADOW_SHADER 1
#define GRAPHICS_TEXTURE_SHADER 2
#define n_shaders 3

class Graphics {
public:

    static GLuint vertex_buffer[n_buffers];
    static GLuint vertex_array[n_buffers];
    static GLuint shaders[n_shaders];

    static Graphics& get();
    void set_mode(GLenum mode);
    void set_colour(RGBA colour);
    void set_shader(int shader);
    void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void fill_rectangle(int x1, int y1, int x2, int y2);

    void flush();
    void reset();
    int check_space();
    int add(GLfloat x, GLfloat y);
    int add(int n, const GLfloat points[]);

private:
    Graphics();
    // Graphics(Graphics const&);          // Don't implement
    // void operator=(Graphics const&);    // Don't implement

    GLfloat buffer[render_buffer_size * 2];
    int p;
    GLenum mode = GL_TRIANGLES;
    RGBA colour;
    int shader;
};

float game_to_gl_x(int x);
float game_to_gl_y(int y);

int initGL();

void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void fill_rectangle(int x1, int y1, int x2, int y2);
#endif	/* GRAPHICS_H */