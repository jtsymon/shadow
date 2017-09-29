#include "A1709View.h"

A1709View::A1709View() {
  printf("initialized A1709\n");
}

A1709View::~A1709View() {
}

void A1709View::show() {}
void A1709View::hide() {}
void A1709View::render() {
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void A1709View::key_callback(int key, int scancode, int action, int mods) {
  GameView::key_callback(key, scancode, action, mods);
}
void A1709View::mouse_callback(int button, int action, int mods) {}

void A1709View::window_size_callback(int width, int height) {

}
