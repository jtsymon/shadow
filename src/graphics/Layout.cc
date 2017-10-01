#include "Layout.h"
#include "Graphics.h"
#include "../misc.h"

int Constraint::absolutePosition() {
  if (this->position_percent) {
    switch (this->edge) {
    case Edge::TOP:
    case Edge::MIDDLE:
    case Edge::BOTTOM:
      return this->position * Graphics::height;
    case Edge::LEFT:
    case Edge::CENTRE:
    case Edge::RIGHT:
      return this->position * Graphics::width;
    default:
      throw new Exception("Invalid edge!");
    }
  } else {
    return this->position;
  }
}

void Layout::validate() {
}

int Layout::getLeft() {
  switch (this->left.edge) {
  case LEFT:
    return this->left.absolutePosition();
  case CENTRE:
    return Graphics::width / 2 + this->left.absolutePosition();
  case RIGHT:
    return Graphics::width - this->left.absolutePosition();
  default:
    throw Exception("Invalid edge!");
  }
}

int Layout::getTop() {
  switch (this->top.edge) {
  case TOP:
    return this->top.absolutePosition();
  case MIDDLE:
    return Graphics::height / 2 + this->top.absolutePosition();
  case BOTTOM:
    return Graphics::height - this->top.absolutePosition();
  default:
    throw Exception("Invalid edge!");
  }
}

int Layout::getRight() {
  switch (this->right.edge) {
  case LEFT:
    return this->right.absolutePosition();
  case CENTRE:
    return Graphics::width / 2 + this->right.absolutePosition();
  case RIGHT:
    return Graphics::width - this->right.absolutePosition();
  default:
    throw Exception("Invalid edge!");
  }
}

int Layout::getBottom() {
  switch (this->bottom.edge) {
  case TOP:
    return this->bottom.absolutePosition();
  case MIDDLE:
    return Graphics::height / 2 + this->bottom.absolutePosition();
  case BOTTOM:
    return Graphics::height - this->bottom.absolutePosition();
  default:
    throw Exception("Invalid edge!");
  }
}

int Layout::getWidth() {
  return this->getRight() - this->getLeft();
}

int Layout::getHeight() {
  return this->getBottom() - this->getTop();
}
