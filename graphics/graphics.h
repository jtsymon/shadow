/* 
 * File:   opengl.h
 * Author: jts
 *
 * Created on 12 January 2014, 8:06 PM
 */

#ifndef OPENGL_H
#define	OPENGL_H

#include <SDL2/SDL_opengl.h>

#include "../main.h"

extern int initGL();

extern void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
extern void fill_rectangle(int x1, int y1, int x2, int y2);
extern void draw_text(int x, int y, char* text);

#endif	/* OPENGL_H */

