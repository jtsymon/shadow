/* 
 * File:   button.h
 * Author: jts
 *
 * Created on 11 January 2014, 11:57 PM
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include "global.h"
#include "graphics/graphics.h"
#include "graphics/font.h"

typedef struct {
    int x, y;
    int w, h;
    char* text;
    int text_x, text_y;
    SDL_Color foreground_color;
    SDL_Color background_color;
    SDL_Color hover_background_color;
    SDL_Color click_background_color;
    font_t* font;
    void (*onclick)();
} button_t;

extern void button_set_position(button_t* this, int x, int y);
extern void button_set_size(button_t* this, int w, int h);
extern void button_set_text(button_t* this, char* text);
extern void button_set_font(button_t* this, font_t* font);

extern button_t* button_create(char* text, int x, int y, void (*onclick)());
extern button_t* button_create_font(char* text, int x, int y, void (*onclick)(), font_t* font);
extern button_t* button_create_size(char* text, int x, int y, void (*onclick)(), int width, int height);
extern button_t* button_create_size_font(char* text, int x, int y, void (*onclick)(), int width, int height, font_t* font);
extern button_t* button_create_size_color(char* text, int x, int y, void (*onclick)(), int width, int height,
        SDL_Color foreground_color, SDL_Color background_color, SDL_Color hover_background_color, SDL_Color click_background_color);
extern button_t* button_create_size_color_font(char* text, int x, int y, void (*onclick)(), int width, int height,
        SDL_Color foreground_color, SDL_Color background_color, SDL_Color hover_background_color, SDL_Color click_background_color,
        font_t* font);

extern bool button_hover(button_t* this, int x, int y);
extern void button_draw(button_t* this);
extern void button_click(button_t* this);


#endif	/* BUTTON_H */

