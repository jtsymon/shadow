#pragma once

class Layout {
 public:
  enum Edge {
    TOP = 0,
    MIDDLE = 1,
    BOTTOM = 2,
    LEFT = 0,
    CENTRE = 1,
    RIGHT = 2
  };

 private:
  int left, top,
      right = -1, bottom = -1;
  Edge left_from = Edge::LEFT, top_from = Edge::TOP,
    right_from = Edge::LEFT, bottom_from = Edge::TOP;
  float width = -1, height = -1;
  bool width_percent = false, height_percent = false;

 public:
  void validate();

 Layout(int x, int y, float w, float h) : left(x), top(y), width(w), height(h) {
    this->validate();
  }

 Layout(int x, int y, float w, float h, bool width_percent, bool height_percent) :
  left(x), top(y), width(w), height(h), width_percent(width_percent), height_percent(height_percent) {
    this->validate();
  }

 Layout(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom) {
    this->validate();
  }

 Layout(int left, Edge left_from, int top, Edge top_from,
        int right, Edge right_from, int bottom, Edge bottom_from) :
  left(left), top(top), right(right), bottom(bottom),
    left_from(left_from), top_from(top_from), right_from(right_from), bottom_from(bottom_from) {
    this->validate();
  }

  int getLeft();
  int getTop();
  int getRight();
  int getBottom();
  int getWidth();
  int getHeight();

  void move(int x, int y);
  void resize(float w, float h, bool width_percent=false, bool height_percent=false);
};
