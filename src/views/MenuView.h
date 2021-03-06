#pragma once

#include "../View.h"
#include "../Button.h"
#include <list>

class MenuView : public View {
  std::list<Button> buttons;
  HSV background_colour;
 public:
  MenuView();
  ~MenuView();
  void show();
  void hide();
  void render();
  void key_callback(int key, int scancode, int action, int mods);
  void mouse_callback(int button, int action, int mods);
  void window_size_callback(int width, int height);
};
