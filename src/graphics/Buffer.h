/* 
 * File:   Buffer.h
 * Author: jts
 *
 * Created on 26 June 2014, 4:33 PM
 */

#ifndef BUFFER_H
#define	BUFFER_H

#include <GL/glew.h>
#include "Texture.h"

class Buffer : public Texture {
    friend class Graphics;
    GLuint framebuffer;
public:
    Buffer(int width, int height);
    Buffer();
    void begin();
    void end();
};

#endif	/* BUFFER_H */

