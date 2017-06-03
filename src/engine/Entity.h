#ifndef ENTITY_H
#define	ENTITY_H

#include <stdio.h>
#include "../math/math.h"
#include "../math/vector.h"
#include "../map/Map.h"

class Entity {
 public:
  Vector<int> pos;
  int radius;
  Map *map;

 Entity(Vector<int> pos, Map *map) : pos(pos), map(map), radius(1000) {
  }
  void move(Vector<int> motion);
};



#endif	/* ENTITY_H */

