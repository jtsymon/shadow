#pragma once

#include "math/vector.h"
extern Vector<double> mouse_pos;
extern Vector<double> mouse_click;

#include "View.h"
extern void push_view(View *view);
extern void pop_view();

#include "graphics/Graphics.h"
extern char mouse_buttons[GLFW_MOUSE_BUTTON_LAST + 1];
extern uint8_t keys[];
extern int keymod;

extern int running;

