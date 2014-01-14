/* 
 * File:   text.h
 * Author: jts
 *
 * Created on 13 January 2014, 12:13 AM
 */

#ifndef TEXT_H
#define	TEXT_H

#include "../main.h"
#include "font.h"

font_t* default_font;
font_t* font_bold_oblique;

typedef struct {
    int w, h;
    char* text;
    font_t* font;
} text_t;

extern bool initFonts();
extern void glPrint(int x, int y, char *string, font_t* font);
extern int text_width(char* text, font_t* font);

#endif	/* TEXT_H */

