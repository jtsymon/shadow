#include "Entity.h"
#include <cmath>
#include <iostream>

void Entity::move(Vector<int> input) {

  std::cout << "Moved by (" << input.x << "," << input.y << ")" << std::endl;

  if (input.x == 0 && input.y == 0) return;

  Vector<int> pos = this->pos.add(input);

  // printf("\nmotion: %d, %d\n", input.x, input.y);

  int rad = this->radius ? this->radius : MAP_SCALE * 10;

  for (MapSegment wall : this->map->segments) {
    LineSegment<int> segment(this->map->points[wall.a], this->map->points[wall.b]);
    bool endpoint;
    double dist = segment.dist(pos, endpoint);
    if (dist < rad) {
      if (endpoint) {
        // if we hit the endpoint and the input is not parallel to the vector,
        // don't bother correcting (because we're going right past it)
        double angle_diff = std::abs(segment.v1.sub(segment.v2).angle() - input.angle());
        if (angle_diff > M_PI_4 && angle_diff < 3 * M_PI_4) {
          continue;
        }
      }
      int side = segment.side(this->pos);
      printf("side: %d; ", side);
      Vector<double> normal = segment.normal(side).normalise();
      printf("normal: (%f, %f)\n", normal.x, normal.y);
      // correct position
      pos = pos.add(normal.scale(rad - dist + 1).toInt());
    }
  }
  // printf("final: %d, %d\n", input.x, input.y);
  this->pos = pos;
}
