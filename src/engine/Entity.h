#pragma once

#include <stdio.h>
#include "../math/math.h"
#include "../math/vector.h"
#include "../map/Map.h"

class Entity {
 public:
  Vector<int> pos;
  int radius;
  Map *map;

 Entity(Vector<int> pos, Map *map) : pos(pos), radius(1000), map(map) {
  }
  void move(Vector<int> motion);
};
