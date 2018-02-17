#pragma once

#include "graphics/Graphics.h"
#include "graphics/BitmapFont.h"
#include "graphics/Layout.h"
#include <string>
#include <functional>

class Button {
  std::string text;
  Layout layout;
 public:
  std::function<void()> onClick;
 private:
  int text_width, text_height;

  void update();

 public:
  RGBA foreground_colour;
  RGBA background_colour;
  RGBA hover_background_colour;
  RGBA click_background_colour;

  BitmapFont *font;

  Button(const std::string &text, float x, float y, std::function<void()> onclick);
  Button(const std::string &text, Layout layout, std::function<void()> onclick);

  void setText(const std::string& text);
  void setFont(BitmapFont *font);

  bool hovering(int x, int y);
  void draw();
  bool click();
  virtual RGBA backgroundColour();
};
