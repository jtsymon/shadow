#ifndef screen_h
#define screen_h

#include <SDL2/SDL.h>

/* Screen_Functions
 *	indexes into vtable of functions a screen has
 */
typedef enum {
	screen_CREATE = 0,	// called when creating the screen
	screen_DESTROY = 1,	// called when destroying the screen
	screen_SHOW = 2,	// called when the screen will start displaying
	screen_HIDE = 3,	// called when the screen will be hidden (but not necessarily destroyed)
	screen_RENDER = 4
} Screen_Functions;

typedef struct {
	void (**f)();
	
	
} screen_t;

#endif
