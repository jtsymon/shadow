/* 
 * File:   global.h
 * Author: jts
 *
 * Created on 14 January 2014, 12:26 PM
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "misc.h"
#include "screen.h"

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
    SDL_GLContext gl_context;
	SDL_Event e;
    int width, height;
} RENDER;

#endif	/* GLOBAL_H */

