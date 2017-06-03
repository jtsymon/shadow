#include "Button.h"
#include "main.h"
#include "graphics/Graphics.h"
#include <string>
#include <iostream>


static const RGBA default_foreground_color = {255, 0, 0, 255};
static const RGBA default_background_color = {200, 200, 200, 255};
static const RGBA default_hover_background_color = {150, 150, 150, 255};
static const RGBA default_click_background_color = {100, 100, 100, 255};

Button::Button(const std::string &text, float x, float y, void (*onclick)()) :
  text(text), layout{Layout(x, y, 150.f, 50.f)}, onclick(onclick),
  foreground_color(default_foreground_color), background_color(default_background_color),
  hover_background_color(default_hover_background_color),
  click_background_color(default_click_background_color),
  font(BitmapFont::standard()) {
    // std::cout << "Button: " << this->x << "," << this->y << " : " << this->w << "," << this->h << std::endl;
    this->update();
                                                 }

Button::Button(const std::string &text, Layout layout, void (*onclick)()) :
  text(text), layout(layout), onclick(onclick),
  foreground_color(default_foreground_color), background_color(default_background_color),
  hover_background_color(default_hover_background_color),
  click_background_color(default_click_background_color),
  font(BitmapFont::standard()) {
  this->update();
}

void Button::update() {
  this->text_x = this->layout.getLeft() + (this->layout.getWidth() - this->font->textWidth(this->text)) / 2;
  this->text_y = this->layout.getTop() + (this->layout.getHeight() - (this->font->getH() / 2)) / 2;
}

void Button::setPosition(int x, int y) {
  this->layout.move(x, y);
  this->update();
}

void Button::setSize(int w, int h) {
  this->layout.resize(w, h);
  this->update();
}

void Button::setText(char *text) {
  this->text = text;
  this->update();
}

void Button::setFont(BitmapFont *font) {
  this->font = font;
  this->update();
}

int Button::hover(int x, int y) {
  return this->layout.getLeft() < x && this->layout.getRight() > x &&
    this->layout.getTop() < y && this->layout.getBottom() > y;
}

void Button::draw() {
  Graphics g = Graphics::get();
  // draw background
  RGBA colour = (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && this->hover(mouse_click.x, mouse_click.y)) ?
    this->click_background_color : (this->hover(mouse_pos.x, mouse_pos.y) ?
                                    this->hover_background_color : this->background_color);
  g.fill_rectangle(colour, this->layout);
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
