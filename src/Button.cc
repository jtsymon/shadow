#include "Button.h"
#include "main.h"
#include "graphics/Graphics.h"
#include <string>
#include <iostream>


static const RGBA default_foreground_colour = {255, 0, 0, 255};
static const RGBA default_background_colour = {200, 200, 200, 255};
static const RGBA default_hover_background_colour = {150, 150, 150, 255};
static const RGBA default_click_background_colour = {100, 100, 100, 255};

Button::Button(const std::string &text, float x, float y, std::function<void()> onClick)
  : text(text), layout(x, y, 150.f, 50.f), onClick(onClick),
    foreground_colour(default_foreground_colour), background_colour(default_background_colour),
    hover_background_colour(default_hover_background_colour),
    click_background_colour(default_click_background_colour),
    font(BitmapFont::standard()) {
  this->update();
}

Button::Button(const std::string &text, Layout layout, std::function<void()> onClick)
  : text(text), layout(layout), onClick(onClick),
    foreground_colour(default_foreground_colour), background_colour(default_background_colour),
    hover_background_colour(default_hover_background_colour),
    click_background_colour(default_click_background_colour),
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

bool Button::hovering(int x, int y) {
  return this->layout.getLeft() < x && this->layout.getRight() > x &&
         this->layout.getTop() < y && this->layout.getBottom() > y;
}

RGBA Button::backgroundColour() {
  if (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] && this->hovering(mouse_click.x, mouse_click.y))
    return this->click_background_colour;
  else if (this->hovering(mouse_pos.x, mouse_pos.y))
    return this->hover_background_colour;
  else
    return this->background_colour;
}

void Button::draw() {
  Graphics g = Graphics::get();
  // draw background
  g.fill_rectangle(this->backgroundColour(), this->layout);
  auto text_x = this->layout.getLeft() + (this->layout.getWidth() - this->text_width) / 2;
  auto text_y = this->layout.getTop() + (this->layout.getHeight() - (this->text_height / 2)) / 2;
  this->font->drawText(text_x, text_y, this->text, this->foreground_colour);
}

bool Button::click() {
  if (this->hovering(mouse_click.x, mouse_click.y) &&
      this->hovering(mouse_pos.x, mouse_pos.y) &&
      this->onClick != NULL) {
    this->onClick();
    return true;
  }
  return false;
}
