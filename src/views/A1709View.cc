#include "A1709View.h"
#include "../keyboard.h"
#include <iostream>

A1709View::A1709View() {
  printf("initialized A1709\n");
}

A1709View::~A1709View() {
}

#define COLUMNS 4

void A1709View::show() {}
void A1709View::hide() {}
void A1709View::render() {
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto g = Graphics::get();
  const int width = Graphics::width * MAP_SCALE,
    height = Graphics::height * MAP_SCALE,
    split = width / COLUMNS;

  for (int i = 0; i < 4; i++) {
    g.fill_rectangle(
                     RGBA(80 * i, 80 * i, 80 * i, 255),
                     Graphics::game_to_gl_x(split * i), Graphics::game_to_gl_y(height - 5000),
                     Graphics::game_to_gl_x(split * i + split), Graphics::game_to_gl_y(height)
                     );
  }

}
void A1709View::key_callback(int key, int scancode, int action, int mods) {
  std::cout << "name=" << key_name(key, scancode) << " key=" << key << " scancode=" << scancode << " action=" << action << " mods=" << mods << std::endl;
  GameView::key_callback(key, scancode, action, mods);
}
void A1709View::mouse_callback(int button, int action, int mods) {}

void A1709View::window_size_callback(int width, int height) {

}
