/* 
 * File:   font.h
 * Author: jts
 *
 * Created on 13 January 2014, 9:14 PM
 */

#ifndef FONT_H
#define	FONT_H

#include "Texture.h"
#include "Graphics.h"
#include <string>

class BitmapFont {
private:
    std::string name;
    int min_char, max_char;
    int tex_size;
    int h;
    int *w;
    GLfloat *chars;

public:
    Texture *texture; // Storage For Our Font Texture
    
    static BitmapFont *standard();
    static BitmapFont *bold_oblique();
    
    static void init();

    BitmapFont(const std::string &name);

    int textWidth(const std::string &text);
    void drawText(int x, int y, const std::string &text, RGBA colour);

    int getH() {
        return this->h;
    }
};


#endif	/* FONT_H */

