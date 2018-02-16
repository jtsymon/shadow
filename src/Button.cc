#include "Button.h"
#include "main.h"
#include "graphics/Graphics.h"
#include <string>
#include <iostream>


static const RGBA default_foreground_color = {255, 0, 0, 255};
static const RGBA default_background_color = {200, 200, 200, 255};
static const RGBA default_hover_background_color = {150, 150, 150, 255};
static const RGBA default_click_background_color = {100, 100, 100, 255};

Button::Button(const std::string &text, float x, float y, std::function<void()> onclick)
  : text(text), layout(x, y, 150.f, 50.f), onclick(onclick),
    foreground_color(default_foreground_color), background_color(default_background_color),
    hover_background_color(default_hover_background_color),
    click_background_color(default_click_background_color),
    font(BitmapFont::standard()) {
  this->update();
}

Button::Button(const std::string &text, Layout layout, std::function<void()> onclick)
  : text(text), layout(layout), onclick(onclick),
    foreground_color(default_foreground_color), background_color(default_background_color),
    hover_background_color(default_hover_background_color),
    click_background_color(default_click_background_color),
    font(BitmapFont::standard()) {
  this->update();
}

void Button::update() {
  this->text_width = this->font->textWidth(this->text);
  this->text_height = this->font->getH();
}

void Button::setText(const std::string& text) {
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
  RGBA colour;
  if (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && this->hover(mouse_click.x, mouse_click.y))
    colour = this->click_background_color;
  else if (this->hover(mouse_pos.x, mouse_pos.y))
    colour = this->hover_background_color;
  else
    colour = this->background_color;
  g.fill_rectangle(colour, this->layout);
  auto text_x = this->layout.getLeft() + (this->layout.getWidth() - this->text_width) / 2;
  auto text_y = this->layout.getTop() + (this->layout.getHeight() - (this->text_height / 2)) / 2;
  this->font->drawText(text_x, text_y, this->text, this->foreground_color);
}

void Button::click() {
  if (this->hover(mouse_click.x, mouse_click.y) &&
      this->hover(mouse_pos.x, mouse_pos.y) &&
      this->onclick != NULL) {
    this->onclick();
  }
}
