/* 
 * File:   buffered.h
 * Author: jts
 *
 * Created on 14 April 2014, 5:39 PM
 */

#ifndef BUFFERED_H
#define	BUFFERED_H

#include "../misc.h"
#include "../global.h"
#include "colour.h"

extern int init_buffer();

extern void buffer_reset();
extern void buffer_end();
extern void buffer_set_mode(GLenum mode);
extern void buffer_set_colour(RGBA colour);
extern void buffer_set_shader(GLuint shader);
extern int buffer_check_space();
extern int buffer_add(GLfloat x, GLfloat y);
extern int buffer_add_n(int n, GLfloat points[]);

#endif	/* BUFFERED_H */

