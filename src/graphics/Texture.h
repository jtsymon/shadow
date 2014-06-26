/* 
 * File:   image.h
 * Author: jts
 *
 * Created on 13 April 2014, 2:02 PM
 */

#ifndef IMAGE_H
#define	IMAGE_H

#include "Colours.h"
#include <string>
#include <cstddef>

class Texture {
    int bit_depth, colour_type;
protected:
    GLuint texture;
    unsigned int width, height;
    Texture(int width, int height) : width(width), height(height) { }
public:
    Texture(const std::string &filename);
    void draw(RGBA colour, int x, int y, int w, int h);
    void drawRegion(RGBA colour, int x, int y, int w, int h, int tx, int ty, int tw, int th);

    GLuint getTexture() {
        return this->texture;
    }
};


#endif	/* IMAGE_H */

