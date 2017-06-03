#include "Shaders.h"
#include <iostream>
#include <string>

/**
 * Display compilation errors from the OpenGL shader compiler
 */
static const std::string error(GLuint object) {
  GLint log_length = 0;
  if (glIsShader(object))
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else if (glIsProgram(object))
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else {
    return "Object was not a shader or a program!";
  }

  char* log = (char*) malloc(log_length);

  if (glIsShader(object))
    glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
    glGetProgramInfoLog(object, log_length, NULL, log);

  std::string message(log);
  free(log);

  return message;
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint compile_shader(const std::string &filename, GLenum type) {

  GLuint shader = glCreateShader(type);

  const char *source = read_file(filename, NULL);

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  delete[] source;

  GLint compile_ok = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE) {
    Exception ex(filename + "\n" + error(shader));
    glDeleteShader(shader);
    throw ex;
  }
  std::cout << "Shader '" << filename << "' compiled successfully" << std::endl;

  return shader;
}

GLuint compile_program(GLuint vertex, GLuint fragment) {

  GLuint program = glCreateProgram();

  // Link the program
  std::cout << "Linking program" << std::endl;
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  // Check the program
  GLint compile_ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE) {
    throw Exception("Failed to link shader program\n" + error(program));
  }

  return program;
}

GLuint compile_program(const std::string &vertex, const std::string &fragment) {
  return compile_program(compile_shader(vertex, GL_VERTEX_SHADER), compile_shader(fragment, GL_FRAGMENT_SHADER));
}
