#pragma once

enum Edge {
  TOP = 0,
  MIDDLE = 1,
  BOTTOM = 2,
  LEFT = 4,
  CENTRE = 8,
  RIGHT = 16
};

struct Constraint {
Constraint(float position, Edge edge, bool position_percent=false) : edge(edge), position(position), position_percent(position_percent) {}
  const Edge edge;
  const float position;
  const bool position_percent;
  int absolutePosition();
};

struct Layout {
private:
  Constraint left, right, top, bottom;

 public:
  void validate();

  Layout(Constraint left, Constraint right, Constraint top, Constraint bottom)
  : left(left), right(right), top(top), bottom(bottom) {
    this->validate();
  }

  Layout(Constraint left, Constraint top, float width, float height)
  : left(left), right(left.position + width, left.edge, left.position_percent),
    top(top), bottom(top.position + height, top.edge, top.position_percent) {
    this->validate();
  }

  Layout(int left, int top, float width, float height)
  : left(left, Edge::LEFT), right(left + width, Edge::LEFT), top(top, Edge::TOP), bottom(top + height, Edge::TOP) {
    this->validate();
  }

  int getLeft();
  int getTop();
  int getRight();
  int getBottom();
  int getWidth();
  int getHeight();
};
