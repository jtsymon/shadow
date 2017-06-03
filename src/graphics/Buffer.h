#ifndef BUFFER_H
#define	BUFFER_H

#include <GL/glew.h>
#include "Texture.h"

class Buffer : public Texture {
  friend class Graphics;
  GLuint framebuffer;
  int prev_width, prev_height;
 public:
  Buffer(int width, int height);
  Buffer();
  void begin();
  void end();
};

#endif	/* BUFFER_H */
