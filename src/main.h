#pragma once

#include "graphics/Graphics.h"
#include "views/MenuView.h"
#include "views/GameView.h"
#include "engine/Resources.h"

extern Vector<double> mouse_pos;
extern Vector<double> mouse_click;
extern char mouse_buttons[GLFW_MOUSE_BUTTON_LAST + 1];
extern uint8_t keys[];
extern int keymod;

extern int running;

extern void push_view(View *view);
extern void pop_view();
