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

font_t* font_default;
font_t* font_bold_oblique;

extern bool initFonts();
extern void glPrint(int x, int y, char *string, font_t* font);
extern int text_height_font(char* text, font_t* font);
extern int text_height(char* text);
extern int text_width_font(char* text, font_t* font);
extern int text_width(char* text);

#endif	/* TEXT_H */

