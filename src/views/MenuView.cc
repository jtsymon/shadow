#include "menu.h"
#include "GameView.h"
#include "../main.h"
#include <iostream>

void play_game() {
  push_view(new GameView);
}

MenuView::MenuView() : background_color(HSV(0, 100, 100)) {
  Button button("Play", 100, 100, play_game);
  this->buttons.push_back(button);
  button = Button("Create map", 100, 160, NULL);
  button.setFont(BitmapFont::bold_oblique());
  this->buttons.push_back(button);
  printf("created menu\n");
}

MenuView::~MenuView() {
  printf("Cleaning up menu screen...\n");
  this->buttons.clear();
}

void MenuView::show() {

}

void MenuView::hide() {

}

void MenuView::render() {

  // background color
  background_color.h++;
  RGB tmp = background_color.toRGB();
  glClearColor((float) tmp.r / 255, (float) tmp.g / 255, (float) tmp.b / 255, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw buttons
  for (Button button : this->buttons) {
    button.draw();
  }

  BitmapFont::standard()->drawText(100, 300, "Testing Text _ <- underscore", RGBA(0, 255, 0, 255));

  BitmapFont::bold_oblique()->texture->draw(RGBA(0, 0, 255, 255), 100, 400, 256, 256);

  BitmapFont::bold_oblique()->texture->drawRegion(RGBA(255, 0, 255, 255), 400, 250, 100, 100, 0, 0, 100, 100);
}

void MenuView::key_callback(int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    running = false;
  }
}

void MenuView::mouse_callback(int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    for (Button button : this->buttons) {
      button.click();
    }
  }
}
