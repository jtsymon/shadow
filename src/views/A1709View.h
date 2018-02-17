#pragma once

#include "GameView.h"
#include "../Button.h"
#include "../keyboard.h"
#include <list>
#include <functional>

class A1709Column : public Button {
 public:
  int key;
  int scancode;
  bool active = false;

  void set_key(int key, int scancode);
  void clickHandler();
  RGBA backgroundColour();

 A1709Column(int key, int scancode, Layout layout)
   : Button(key_name(key, scancode), layout, [this]() { this->clickHandler(); }),
    key(key), scancode(scancode) {
  }
};

class A1709View : public GameView {
  std::list<A1709Column> columns;
  A1709Column *configuring = NULL;
 public:
  A1709View();
  ~A1709View();
  void show();
  void hide();
  void render();
  void key_callback(int key, int scancode, int action, int mods);
  void mouse_callback(int button, int action, int mods);
  void window_size_callback(int width, int height);
};

