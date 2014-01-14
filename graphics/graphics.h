/* 
 * File:   opengl.h
 * Author: jts
 *
 * Created on 12 January 2014, 8:06 PM
 */

#ifndef OPENGL_H
#define	OPENGL_H

#include "../main.h"
#include "text.h"
#include "font.h"

double two_over_width, two_over_height;

extern double game_to_gl_x(int x);
extern double game_to_gl_y(int y);

extern int initGL();

extern void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
extern void fill_rectangle(int x1, int y1, int x2, int y2);
extern void draw_texture(int x, int y, int w, int h);
extern void draw_text_font(int x, int y, char* text, font_t* font);
extern void draw_text(int x, int y, char* text);

#endif	/* OPENGL_H */

