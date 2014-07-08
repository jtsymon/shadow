/* 
 * File:   main.h
 * Author: jts
 *
 * Created on 10 Nov 2013
 */

#ifndef MAIN_H
#define MAIN_H

#include "graphics/Graphics.h"
#include "views/MenuView.h"
#include "views/GameView.h"
#include "engine/Resources.h"

extern Vector<double> mouse_pos;
extern Vector<double> mouse_click;
extern char mouse_buttons[GLFW_MOUSE_BUTTON_LAST + 1];
extern uint8_t keys[];
extern int keymod;

extern MenuView *menu_view;
extern GameView *game_view;

extern int running;

extern void set_view(View *new_view);
#endif /* MAIN_H */
