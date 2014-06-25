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


extern GLuint compile_shader(const std::string &filename, GLenum type);
extern GLuint compile_program(GLuint vertex, GLuint fragment);
extern GLuint compile_program(const std::string &vertex, const std::string &fragment);


#endif	/* SHADERS_H */

