#ifndef SCREEN_H
#define SCREEN_H

class View {
 public:
  virtual void show() = 0;
  virtual void hide() = 0;
  virtual void render() = 0;
  virtual void key_callback(int key, int scancode, int action, int mods) = 0;
  virtual void mouse_callback(int button, int action, int mods) = 0;
};


#endif /* SCREEN_H */
