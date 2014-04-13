/* 
 * File:   colour.h
 * Author: jts
 *
 * Created on 13 April 2014, 2:28 PM
 */

#ifndef COLOUR_H
#define	COLOUR_H

#include "../misc.h"

typedef struct {
    GLubyte r;
    GLubyte g;
    GLubyte b;
} RGB;

typedef struct {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} RGBA;

typedef struct {
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HSV;

extern RGB HsvToRgb(HSV hsv);
extern HSV RgbToHsv(RGB rgb);

#endif	/* COLOUR_H */

