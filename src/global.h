/* 
 * File:   global.h
 * Author: jts
 *
 * Created on 14 January 2014, 12:26 PM
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "misc.h"
#include "math/math.h"
#include "screen.h"

// commonly used variables
// (used frequently enough to warrant keeping them outside of a struct)
uint8_t keys[GLFW_KEY_LAST + 1];
int keymod;
bool running;
struct {
    char buttons[GLFW_MOUSE_BUTTON_LAST + 1];
    double x, y;
    double sx, sy;
} mouse;

// contains general stuff that needs to be globally accessible
struct {
	screen_t* screen_menu;
	screen_t* screen_game;
	int sleep_time;
	uint32_t last_tick;
} GLOBALS;

GLuint program;

#define n_buffers 2

// contains stuff used by screens to render
struct {
    GLFWwindow* window;
    int width, height;
    struct {
        GLuint texture;
        GLuint framebuffer;
        GLuint vertex_buffer;
        GLuint vertex_array;
    } mask;
    struct {
        GLuint shadow;
        GLuint texture;
        GLuint colour;
    } shaders;
    GLuint vertex_buffer[n_buffers];
    GLuint vertex_array[n_buffers];
} RENDER;

#endif	/* GLOBAL_H */

