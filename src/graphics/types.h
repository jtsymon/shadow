/* 
 * File:   types.h
 * Author: jts
 *
 * Created on 17 April 2014, 9:24 PM
 */

#ifndef TYPES_H
#define	TYPES_H

#include "../misc.h"

typedef struct {
    GLuint texture;
    int width, height;
    int bit_depth, colour_type;
} texture_t;

typedef struct {
    char *name;
    int min_char, max_char;
    int tex_size;
    int h;
    int *w;
    GLfloat *chars;
    texture_t texture;  // Storage For Our Font Texture
} font_t;

#endif	/* TYPES_H */

