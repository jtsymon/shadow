#pragma once

#include "graphics/Graphics.h"
#include "graphics/BitmapFont.h"
#include "graphics/Layout.h"
#include <string>

class Button {
  std::string text;
  Layout layout;
  void (*onclick)();
  int text_x, text_y;
  RGBA foreground_color;
  RGBA background_color;
  RGBA hover_background_color;
  RGBA click_background_color;
  BitmapFont *font;

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
