/* 
 * File:   main.h
 * Author: jts
 *
 * Created on 10 Nov 2013
 */

#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "misc.h"
#include "screens/menu.h"
#include "screens/game.h"
#include "map.h"

// commonly used variables
// (used frequently enough to warrant keeping them outside of a struct)
const Uint8* keys;
SDL_Keymod keymod;
SDL_bool running;
struct {
    int x, y;
    bool buttons[6];
    SDL_Point click_start;
} mouse;

// contains general stuff that needs to be globally accessible
struct {
	screen_t* screen_menu;
	screen_t* screen_game;
	int sleep_time;
	Uint32 last_tick;
} GLOBALS;

// contains stuff used by screens to render
struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
	SDL_Event e;
	TTF_Font* font;
} RENDER;

extern void set_screen(screen_t* newScreen);

#endif /* MAIN_H */
