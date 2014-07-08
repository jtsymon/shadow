/* 
 * File:   button.h
 * Author: jts
 *
 * Created on 11 January 2014, 11:57 PM
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include "graphics/Graphics.h"
#include "graphics/BitmapFont.h"
#include "graphics/Layout.h"
#include <string>

class Button {
    Layout layout;
    std::string text;
    int text_x, text_y;
    RGBA foreground_color;
    RGBA background_color;
    RGBA hover_background_color;
    RGBA click_background_color;
    BitmapFont *font;
    void (*onclick)();

    void update();

public:
    Button(const std::string &text, float x, float y, void (*onclick)());
    Button(const std::string &text, Layout layout, void (*onclick)());

    void setPosition(int x, int y);
    void setSize(int w, int h);
    void setText(char *text);
    void setFont(BitmapFont *font);

    int hover(int x, int y);
    void draw();
    void click();
};


#endif	/* BUTTON_H */

