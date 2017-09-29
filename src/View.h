#pragma once

class View {
 public:
  virtual void show() = 0;
  virtual void hide() = 0;
  virtual void render() = 0;
  virtual void key_callback(int key, int scancode, int action, int mods) = 0;
  virtual void mouse_callback(int button, int action, int mods) = 0;
  virtual ~View() {}
};
