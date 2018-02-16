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
  std::function<void()> onclick;
 private:
  int text_width, text_height;

  void update();

 public:
  RGBA foreground_color;
  RGBA background_color;
  RGBA hover_background_color;
  RGBA click_background_color;

  BitmapFont *font;

  Button(const std::string &text, float x, float y, std::function<void()> onclick);
  Button(const std::string &text, Layout layout, std::function<void()> onclick);

  void setText(const std::string& text);
  void setFont(BitmapFont *font);

  int hover(int x, int y);
  void draw();
  void click();
};
