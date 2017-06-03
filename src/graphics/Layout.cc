#include "Layout.h"
#include "Graphics.h"
#include "../misc.h"

void Layout::validate() {
  if (this->width == -1 && this->right == -1)
    throw Exception("Cannot determine dimensions (width or right-side must be defined)");
  if (this->height == -1 && this->bottom == -1)
    throw Exception("Cannot determine dimensions (height or bottom-side must be defined)");
}

int Layout::getLeft() {
  switch (this->left_from) {
  case LEFT:
    return this->left;
  case CENTRE:
    return Graphics::width / 2 + this->left;
  case RIGHT:
    return Graphics::width - this->left;
  default:
    throw Exception("Invalid edge!");
  }
}

int Layout::getTop() {
  switch (this->top_from) {
  case TOP:
    return this->top;
  case MIDDLE:
    return Graphics::height / 2 + this->top;
  case BOTTOM:
    return Graphics::height - this->top;
  default:
    throw Exception("Invalid edge!");
  }
}

int Layout::getRight() {
  if (this->right == -1) {
    return this->getLeft() + this->getWidth();
  }
  switch (this->right_from) {
  case LEFT:
    return this->right;
  case CENTRE:
    return Graphics::width / 2 + this->right;
  case RIGHT:
    return Graphics::width - this->right;
  default:
    throw Exception("Invalid edge!");
  }
}

int Layout::getBottom() {
  if (this->bottom == -1) {
    return this->getTop() + this->getHeight();
  }
  switch (this->bottom_from) {
  case TOP:
    return this->bottom;
  case MIDDLE:
    return Graphics::height / 2 + this->bottom;
  case BOTTOM:
    return Graphics::height - this->bottom;
  default:
    throw Exception("Invalid edge!");
  }
}

int Layout::getWidth() {
  if (this->width == -1) {
    return this->getRight() - this->getLeft();
  }
  if (this->width_percent) {
    return this->width * Graphics::width;
  }
  return this->width;
}

int Layout::getHeight() {
  if (this->height == -1) {
    return this->getBottom() - this->getTop();
  }
  if (this->height_percent) {
    return this->height * Graphics::height;
  }
  return this->height;
}

void Layout::move(int x, int y) {
  int dx = this->left - x, dy = this->top - y;
  this->left = x;
  this->top = top;
  if (this->right != -1 && this->left_from == this->right_from) {
    this->right += dx;
  }
  if (this->bottom != -1 && this->top_from == this->bottom_from) {
    this->bottom += dy;
  }
}

void Layout::resize(float w, float h, bool width_percent, bool height_percent) {
  this->right = -1;
  this->bottom = -1;
  this->width = w;
  this->height = h;
  this->width_percent = width_percent;
  this->height_percent = height_percent;
}
