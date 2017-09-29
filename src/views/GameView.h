#pragma once

#include "../View.h"
#include "../main.h"

class GameView : public View {
 public:
  virtual void key_callback(int key, int scancode, int action, int mods) {
    if(action == GLFW_RELEASE) {
      switch(key) {
      case GLFW_KEY_ESCAPE:
        pop_view();
        break;
      }
    }
  }
};
