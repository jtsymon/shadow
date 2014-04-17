/* 
 * File:   image.h
 * Author: jts
 *
 * Created on 13 April 2014, 2:02 PM
 */

#ifndef IMAGE_H
#define	IMAGE_H

#include <png.h>
#include <string.h>
#include "graphics.h"
#include "../misc.h"

extern GLubyte *load_png(char *name, int *width, int *height, int *bit_depth, int *colour_type);
extern texture_t texture_png(char *filename);
extern void draw_texture(texture_t texture, RGBA colour, int x, int y, int w, int h);
extern void draw_texture_region(texture_t texture, RGBA colour, int x, int y, int w, int h, int tx, int ty, int tw, int th);

#endif	/* IMAGE_H */

