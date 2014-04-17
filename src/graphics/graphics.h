/* 
 * File:   graphics.h
 * Author: jts
 *
 * Created on 12 January 2014, 8:06 PM
 */

#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include "types.h"
#include "../misc.h"
#include "colour.h"
#include "text.h"
#include "font.h"
#include "shaders.h"
#include "buffered.h"

double two_over_width, two_over_height;

extern double game_to_gl_x(int x);
extern double game_to_gl_y(int y);

extern int initGL();

extern void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
extern void fill_rectangle(int x1, int y1, int x2, int y2);

#endif	/* GRAPHICS_H */

