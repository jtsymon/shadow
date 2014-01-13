/* 
 * File:   font.h
 * Author: jts
 *
 * Created on 13 January 2014, 9:14 PM
 */

#ifndef FONT_H
#define	FONT_H

#include "graphics.h"

typedef struct {
    int x, y, w, h;
} font_char;

typedef struct {
    char* name;
    int min_char, max_char;
    int tex_size;
    font_char* chars;
    GLuint base;        // Base Display List For The Font
    GLuint texture;     // Storage For Our Font Texture
} font_t;

extern font_t* font_open(char* name);

#endif	/* FONT_H */

