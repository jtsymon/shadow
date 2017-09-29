#pragma once

#include "GameView.h"

class A1709View : public GameView {
 public:
  A1709View();
  ~A1709View();
  void show();
  void hide();
  void render();
  void key_callback(int key, int scancode, int action, int mods);
  void mouse_callback(int button, int action, int mods);
};
