/* 
 * File:   misc.h
 * Author: jts
 *
 * Created on 11 January 2014, 2:42 PM
 */


#ifndef MISC_H
#define	MISC_H

#include <GL/glew.h>
#include <GL/glxew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

const char* read_file(const std::string &filename, int *size);

struct Exception : public std::runtime_error {

    Exception(std::string const& message) : std::runtime_error("Error: " + message) {
    }
};


#endif	/* MISC_H */

