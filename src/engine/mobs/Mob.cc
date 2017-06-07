#include "Mob.h"
#include <list>
#include <iostream>

void Mob::tick(Vector<int> target) {
  std::list<Vector<int>> path = this->map->pathfinder.Dijkstra(this->pos, target);
  path.pop_front();
  Vector<int> move = (path.front() - this->pos).toDouble().normalise(MAP_SCALE).toInt();
  this->move(move);

  std::cout << "Mob moved by (" << move.x << "," << move.y << ")" << std::endl;
}
