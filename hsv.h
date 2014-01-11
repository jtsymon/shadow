/* 
 * File:   hsv.h
 * Author: jts
 *
 * Created on 12 January 2014, 2:22 AM
 */

#ifndef HSV_H
#define	HSV_H

typedef struct {
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

extern SDL_Color HsvToRgb(HsvColor hsv);
extern HsvColor RgbToHsv(SDL_Color rgb);

#endif	/* HSV_H */

