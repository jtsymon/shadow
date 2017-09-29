#pragma once

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
