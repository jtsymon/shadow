#pragma once

#include "../math/vector.h"
#include "types.h"
#include <vector>
#include <list>
#include <utility>

class Map;

class PathFinder {
  Map *map;
  std::vector<std::pair<int, int>> connections(Vector<int> pos);
 public:
  std::vector<Vector<int>>                        path_nodes;
  std::vector<std::vector<std::pair<int, int>>>   path_connections;

  PathFinder() { }
  PathFinder(Map *map);
  std::list<Vector<int>> Dijkstra(Vector<int> from, Vector<int> to);
  void update_dynamic(); // generates temporary path nodes from dynamic map elements
};
