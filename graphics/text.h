/* 
 * File:   text.h
 * Author: jts
 *
 * Created on 13 January 2014, 12:13 AM
 */

#ifndef TEXT_H
#define	TEXT_H

#include "graphics.h"
#include "font.h"

font_t* font;

extern bool initFont();
extern void glPrint(int x, int y, char *string);

#endif	/* TEXT_H */

