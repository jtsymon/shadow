#include "Entity.h"
#include <cmath>
#include <iostream>

void Entity::move(Vector<int> input) {

  if (input.x == 0 && input.y == 0) return;

  Vector<int> start = pos, end = pos + input;

  int rad = radius ? radius : MAP_SCALE * 10;

  for (MapSegment wall : this->map->segments) {

    LineSegment<int> segment(this->map->points[wall.a], this->map->points[wall.b]);
    LineSegment<int> movement(start, end);
    // stop fast movement from jumping walls
    if (segment.intersects(movement) && !(end == segment.v1 || end == segment.v2)) {
      double x12 = segment.v1.x - segment.v2.x,
        x34 = movement.v1.x - movement.v2.x,
        y12 = segment.v1.y - segment.v2.y,
        y34 = movement.v1.y - movement.v2.y,
        c = x12 * y34 - y12 * x34,
        a = (double)segment.v1.x * segment.v2.y - (double)segment.v1.y * segment.v2.x,
        b = (double)movement.v1.x * movement.v2.y - (double)movement.v1.y * movement.v2.x;

      Vector<int> intersection((int)((a * x34 - b * x12) / c),
                               (int)((a * y34 - b * y12) / c));

      if (segment.on_line(intersection)) {
        end = intersection;
      }
    }
    bool endpoint;
    double dist = segment.dist(end, endpoint);
    if (dist < rad) {
      if (endpoint) {
        // if we hit the endpoint and the input is not parallel to the vector,
        // don't bother correcting (because we're going right past it)
        double angle_diff = fmod(std::abs((segment.v1 - segment.v2).angle() - input.angle()), M_PI);
        if (angle_diff > M_PI / 8 && angle_diff < M_PI * 7/8) {
          continue;
        }
      }
      int side = segment.side(start);
      Vector<double> normal = segment.normal(side).normalise();
      // correct position
      end += (normal * (rad - dist + 1)).toInt();
    }
  }

  auto shift = end - start;
  std::cout << "Moved by (" << shift.x << "," << shift.y << ")" << std::endl;

  this->pos = end;
}
