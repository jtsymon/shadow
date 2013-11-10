#ifndef main_h
#define main_h

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>

#include "screens/menu.h"
#include "screens/game.h"

// commonly used variables
// (used frequently enough to warrant keeping them outside of a struct)
const Uint8* keys;
SDL_Keymod keymod;
SDL_bool running;
screen_t* screen;

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

#endif
