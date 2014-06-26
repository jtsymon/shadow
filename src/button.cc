#include "button.h"
#include "main.h"
#include "graphics/Graphics.h"
#include <string>
#include <iostream>


static const RGBA default_foreground_color = {255, 0, 0, 255};
static const RGBA default_background_color = {200, 200, 200, 255};
static const RGBA default_hover_background_color = {150, 150, 150, 255};
static const RGBA default_click_background_color = {100, 100, 100, 255};

Button::Button(const std::string &text, int x, int y, void (*onclick)()) :
text(text), x(x), y(y), onclick(onclick), w(150), h(50),
foreground_color(default_foreground_color), background_color(default_background_color),
hover_background_color(default_hover_background_color),
click_background_color(default_click_background_color),
font(BitmapFont::standard()) {
    std::cout << "Button: " << this->x << "," << this->y << " : " << this->w << "," << this->h << std::endl;
    this->layout();
}

Button::Button(const std::string &text, int x, int y, void (*onclick)(), int width, int height) :
text(text), x(x), y(y), onclick(onclick), w(w), h(h),
foreground_color(default_foreground_color), background_color(default_background_color),
hover_background_color(default_hover_background_color),
click_background_color(default_click_background_color),
font(BitmapFont::standard()) {
    this->layout();
}

void Button::layout() {
    this->text_x = this->x + (this->w - this->font->textWidth(this->text)) / 2;
    this->text_y = this->y + (this->h - (this->font->getH() / 2)) / 2;
}

void Button::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    this->layout();
}

void Button::setSize(int w, int h) {
    this->w = w;
    this->h = h;
    this->layout();
}

void Button::setText(char *text) {
    this->text = text;
    this->layout();
}

void Button::setFont(BitmapFont *font) {
    this->font = font;
    this->layout();
}

int Button::hover(int x, int y) {
    return this->x < x && this->x + this->w > x && this->y < y && this->y + this->h > y;
}

void Button::draw() {
    Graphics g = Graphics::get();
    // draw background
    RGBA colour = (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && this->hover(mouse_click.x, mouse_click.y)) ?
        this->click_background_color : (this->hover(mouse_pos.x, mouse_pos.y) ?
            this->hover_background_color : this->background_color);
    g.fill_rectangle(colour, this->x, this->y, this->x + this->w, this->y + this->h);
    // draw text
    this->font->drawText(this->text_x, this->text_y, this->text, this->foreground_color);
}

void Button::click() {
    if (this->hover(mouse_click.x, mouse_click.y) &&
            this->hover(mouse_pos.x, mouse_pos.y) &&
            this->onclick != NULL) {
        this->onclick();
    }
}
