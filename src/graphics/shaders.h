/* 
 * File:   shaders.h
 * Author: jts
 *
 * Created on 13 April 2014, 4:38 PM
 */

#ifndef SHADERS_H
#define	SHADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <GL/glew.h>
#include "../misc.h"

extern void shader_error(GLuint object);
extern GLuint create_shader(const char* filename, GLenum type);
extern GLuint create_program(GLuint vertex_shader, GLuint fragment_shader);
extern GLuint create_program_src(const char * vertex_file_path, const char * fragment_file_path);

#endif	/* SHADERS_H */

