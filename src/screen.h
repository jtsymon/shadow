/* 
 * File:   screen.h
 * Author: jts
 *
 * Created on 10 Nov 2013
 */

#ifndef SCREEN_H
#define SCREEN_H

/* Screen_Functions
 *	indexes into vtable of functions a screen has
 */
enum {
	screen_CREATE = 0,      // called when creating the screen
	screen_DESTROY,         // called when destroying the screen
	screen_SHOW,            // called when the screen will start displaying
	screen_HIDE,            // called when the screen will be hidden (but not necessarily destroyed)
	screen_RENDER,          // called when the screen should draw
    screen_KEY_CALLBACK,    // called when a key event is received
    screen_MOUSE_CALLBACK   // called when a mouse event is received
};

typedef struct {
	void (**f)();
} screen_t;

#endif /* SCREEN_H */
