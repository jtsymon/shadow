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

#define GRAPHICS_COLOUR_SHADER 0
#define GRAPHICS_SHADOW_SHADER 1
#define GRAPHICS_TEXTURE_SHADER 2
#define GRAPHICS_BLUR_SHADER 3
#define n_shaders 4

class Batch {
    friend class Graphics;
    GLuint mode;
    RGBA colour;
    GLuint shader;
    int size;
    int p = 0;
public:
    GLfloat *buffer;
    Batch(GLuint mode, RGBA colour, GLuint shader, int size=1024) :
            mode(mode), colour(colour), shader(shader), size(size),
            buffer(new GLfloat[size * 2]) { }
    ~Batch() {
        if (this->buffer) {
            delete this->buffer;
            this->buffer = NULL;
        }
    }
    int add(GLfloat x, GLfloat y);
    int add(int n, const GLfloat points[]);
    int check_space();
    void clear();
};

class Graphics {
public:

    static GLuint vertex_buffer[n_buffers];
    static GLuint vertex_array[n_buffers];
    static GLuint shaders[n_shaders];

    static Graphics& get();
    void draw(RGBA colour, GLuint mode, GLuint shader, int n, GLfloat *points);
    void draw(Batch &batch);
    void fill_triangle(RGBA colour, int x1, int y1, int x2, int y2, int x3, int y3);
    void fill_rectangle(RGBA colour, int x1, int y1, int x2, int y2);

private:
    Graphics();
    // Graphics(Graphics const&);          // Don't implement
    // void operator=(Graphics const&);    // Don't implement
};

float game_to_gl_x(int x);
float game_to_gl_y(int y);

int initGL();

void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void fill_rectangle(int x1, int y1, int x2, int y2);
#endif	/* GRAPHICS_H */