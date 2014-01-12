/* 
 * File:   button.h
 * Author: jts
 *
 * Created on 11 January 2014, 11:57 PM
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include "main.h"

typedef struct {
    int x;
    int y;
    int width;
    int height;
    char* text;
    SDL_Color foreground_color;
    SDL_Color background_color;
    SDL_Color hover_background_color;
    SDL_Color click_background_color;
    void (*onclick)();
} button_t;


extern button_t* button_create(char* text, int x, int y, void (*onclick)());
extern button_t* button_create_(char* text, int x, int y, void (*onclick)(), int width, int height);
extern button_t* button_create__(char* text, int x, int y, void (*onclick)(), int width, int height,
        SDL_Color foreground_color, SDL_Color background_color, SDL_Color hover_background_color, SDL_Color click_background_color);

extern bool button_hover(button_t* this, int x, int y);
extern void button_draw(button_t* this);
extern void button_click(button_t* this);


#endif	/* BUTTON_H */

