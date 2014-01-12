/* 
 * File:   screen.h
 * Author: jts
 *
 * Created on 10 Nov 2013
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>

#include "misc.h"

/* Screen_Functions
 *	indexes into vtable of functions a screen has
 */
typedef enum {
	screen_CREATE = 0,	// called when creating the screen
	screen_DESTROY = 1,	// called when destroying the screen
	screen_SHOW = 2,	// called when the screen will start displaying
	screen_HIDE = 3,	// called when the screen will be hidden (but not necessarily destroyed)
	screen_RENDER = 4   // called when the screen should draw
} Screen_Functions;

typedef struct {
	void (**f)();
} screen_t;

#endif /* SCREEN_H */
