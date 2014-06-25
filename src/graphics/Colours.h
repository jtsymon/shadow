/* 
 * File:   colour.h
 * Author: jts
 *
 * Created on 13 April 2014, 2:28 PM
 */

#ifndef COLOUR_H
#define	COLOUR_H

#include "../misc.h"

struct HSV;

struct RGB {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    
    RGB(GLubyte r, GLubyte g, GLubyte b) : r(r), g(g), b(b) { }
    
    HSV toHSV();
};

struct RGBA {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
    
    RGBA(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : r(r), g(g), b(b), a(a) { }
};

struct HSV {
    unsigned char h;
    unsigned char s;
    unsigned char v;
    
    HSV(unsigned char h, unsigned char s, unsigned char v) : h(h), s(s), v(v) { }
    
    RGB toRGB();
};

#endif	/* COLOUR_H */

