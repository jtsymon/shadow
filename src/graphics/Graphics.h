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
#include "Layout.h"

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
    friend class Layout;
public:
    static float two_over_width, two_over_height;
    static int width, height;
    
    static GLuint vertex_buffer[n_buffers];
    static GLuint vertex_array[n_buffers];
    static GLuint shaders[n_shaders];

    static Graphics& get(int width=0, int height=0);
    
    void draw(RGBA colour, GLuint mode, GLuint shader, int n, GLfloat *points);
    void draw(Batch &batch);
    void fill_triangle(RGBA colour, float x1, float y1, float x2, float y2, float x3, float y3);
    void fill_rectangle(RGBA colour, float x1, float y1, float x2, float y2);
    void fill_rectangle(RGBA colour, Layout layout);
    static float window_to_gl_x(int x);
    static float window_to_gl_y(int y);
    static float game_to_gl_x(int x);
    static float game_to_gl_y(int y);
    static void update_dimensions(int width, int height);

private:
    Graphics(int width, int height);
    // Graphics(Graphics const&);          // Don't implement
    // void operator=(Graphics const&);    // Don't implement
};

int initGL();

void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void fill_rectangle(int x1, int y1, int x2, int y2);
#endif	/* GRAPHICS_H */