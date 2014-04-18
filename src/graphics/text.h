/* 
 * File:   text.h
 * Author: jts
 *
 * Created on 13 January 2014, 12:13 AM
 */

#ifndef TEXT_H
#define	TEXT_H

#include "font.h"
#include "colour.h"

font_t* default_font;
font_t* font_bold_oblique;

extern int init_fonts();
extern void draw_text_f(int x, int y, char* text, RGBA colour, font_t* font);
extern void draw_text(int x, int y, char* text, RGBA colour);
extern int text_width(char* text, font_t* font);

#endif	/* TEXT_H */

