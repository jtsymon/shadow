/* 
 * File:   screen.h
 * Author: jts
 *
 * Created on 10 Nov 2013
 */

#ifndef SCREEN_H
#define SCREEN_H

typedef struct {
	void (*create)(),               // called when creating the screen
            (*destroy)(),           // called when destroying the screen
            (*show)(),              // called when the screen will start displaying
            (*hide)(),              // called when the screen will be hidden (but not necessarily destroyed)
            (*render)(),            // called when the screen should draw
            (*key_callback)(),      // called when a key event is received
            (*mouse_callback)();    // called when a mouse event is received
} screen_t;

#endif /* SCREEN_H */
