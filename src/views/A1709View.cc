#include "A1709View.h"
#include "../keyboard.h"
#include <iostream>

#define COLUMNS 4

static const RGBA active_background_colour = {0, 100, 200, 255};

void A1709Column::clickHandler() {
}

void A1709Column::set_key(int key, int scancode) {
  this->key = key;
  this->scancode = scancode;
  this->setText(key_name(key, scancode));
}

RGBA A1709Column::backgroundColour() {
  if (this->active)
    return active_background_colour;
  else
    return Button::backgroundColour();
}

Layout columnConstraint(float x) {
  return Layout(Constraint(x, Edge::LEFT, true),
                Constraint(x + 0.25, Edge::LEFT, true),
                Constraint(40, Edge::BOTTOM),
                Constraint(0, Edge::BOTTOM));
}

A1709View::A1709View() {
  float x = 0;
  this->columns.push_back(A1709Column('Z', 0, columnConstraint(x)));
  x += 0.25;
  this->columns.push_back(A1709Column('X', 0, columnConstraint(x)));
  x += 0.25;
  this->columns.push_back(A1709Column('C', 0, columnConstraint(x)));
  x += 0.25;
  this->columns.push_back(A1709Column('V', 0, columnConstraint(x)));
  x += 0.25;
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

  for (auto column : this->columns) {
    column.draw();
  }
}

void A1709View::key_callback(int key, int scancode, int action, int mods) {
  std::cout << "name=" << key_name(key, scancode)
            << " key=" << key
            << " scancode=" << scancode
            << " action=" << action
            << " mods=" << mods
            << std::endl;
  int i = 1;
  if (this->configuring) {
    bool canConfigure = true;
    for (auto& column : this->columns) {
      if (&column != this->configuring &&
          ((column.key != 0 && key == column.key) ||
           (column.scancode != 0 && scancode == column.scancode))) {
        canConfigure = false;
        break;
      }
    }
    if (canConfigure) {
      this->configuring->set_key(key, scancode);
    } else {
      std::cout << "Cannot set multiple columns to the same key!" << std::endl;
    }
    this->configuring = NULL;
  } else {
    for (auto& column : this->columns) {
      if ((column.key != 0 && key == column.key) || (column.scancode != 0 && scancode == column.scancode))
        column.active = (action != 0);
      i++;
    }
  }
  GameView::key_callback(key, scancode, action, mods);
}
void A1709View::mouse_callback(int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    for (auto& column : this->columns) {
      if (column.click())
        this->configuring = &column;
    }
  }
}

void A1709View::window_size_callback(int width, int height) {

}
