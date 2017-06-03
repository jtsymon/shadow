#include "PathFinder.h"
#include "Map.h"
#include <iostream>
#include <algorithm>
#include <utility>
#include <list>
#include <queue>

#define path_node_dist MAP_SCALE

static bool node_sorter(std::pair<WallConnection*, double> first, std::pair<WallConnection*, double> second) {
  return first.second < second.second;
}

PathFinder::PathFinder(Map *map) : map(map) {
  // generate nodes
  for (WallConnection point : map->points) {
    int size = point.connected.size();
    if (size == 1) {
      // single line case
      this->path_nodes.push_back(point.add(point.sub(*point.connected[0]).toDouble().normalise().scale(path_node_dist).toInt()));
    } else if (size > 1) {
      // meeting of multiple lines case
      std::list<std::pair < WallConnection*, double>> sorted;
      for (WallConnection *end : point.connected) {
        sorted.push_back(std::pair<WallConnection*, double>(end, point.sub(*end).angle()));
      }
      // sort the connected nodes in order of angle
      sorted.sort(node_sorter);

      std::pair<WallConnection*, double> prev = sorted.back();
      for (std::pair<WallConnection*, double> next : sorted) {
        // get the vector between each pair of adjacent nodes in the sorted list
        // get the normal of that vector
        // add that to the central point as with the single line case
        LineSegment<int> line(*next.first, *prev.first);
        Vector<int> normal = line.normal(line.side(point)).normalise().scale(path_node_dist).toInt();

        Vector<int> node = point.add(normal);
        bool do_add = true;

        for (Vector<int> existing : this->path_nodes) {
          if (node == existing) {
            do_add = false;
            break;
          }
        }
        if (do_add) {
          this->path_nodes.push_back(node);
        }

        prev = next;
      }
      sorted.clear();
    } else {
      // single point, invalid
      throw Exception("Encountered lone point, invalid!");
    }
  }

  int size = this->path_nodes.size();
  for (Vector<int> node : this->path_nodes) {
    std::cout << "path_node (" << node.x << "," << node.y << ")" << std::endl;
  }
  std::cout << size << " path nodes" << std::endl;

  // create vectors for connections
  for (int i = 0; i < size; i++) {
    std::vector<std::pair<int, int>> path_connections;
    this->path_connections.push_back(path_connections);
  }

  int connections = 0;
  // find connections between nodes
  for (int i = 0; i < size; i++) {
    Vector<int> point = this->path_nodes[i];
    for (int j = i; j < size; j++) {
      Vector<int> endpoint = this->path_nodes[j];
      if (point.x == endpoint.x && point.y == endpoint.y) continue;
      if (map->can_see(point, endpoint)) {
        std::cout << "(" << point.x << "," << point.y << ") can see (" << endpoint.x << "," << endpoint.y << ")" << std::endl;
        int dist = (int)point.dist(endpoint);
        this->path_connections[i].push_back(std::pair<int, int>(j, dist));
        this->path_connections[j].push_back(std::pair<int, int>(i, dist));
        connections += 2;
      }
    }

    // std::sort(this->path_nodes[i].connected.begin(), this->path_nodes[i].connected.end(), path_sorter);
  }

  std::cout << "generated visibility graph (" << connections << " connections)" << std::endl;
}

std::vector<std::pair<int, int>> PathFinder::connections(Vector<int> pos) {
  std::vector<std::pair<int, int>> connected;
  int size = this->path_nodes.size();
  for (int i = 0; i < size; i++) {
    Vector<int> endpoint = this->path_nodes[i];
    if (pos.x == endpoint.x && pos.y == endpoint.y) continue;
    if (this->map->can_see(pos, endpoint)) {
      int dist = (int)pos.dist(endpoint);
      connected.push_back(std::pair<int, int>(i, dist));
    }
  }
  return connected;
}

struct comp {
  bool operator() (const std::pair<int, int> &a, const std::pair<int, int> &b) {
    return a.second > b.second;
  }
};

std::list<Vector<int>> PathFinder::Dijkstra(Vector<int> from, Vector<int> to) {

  if (this->map->can_see(from, to)) {
    return std::list<Vector<int>>({
        from, to
          });
  }

  // find the possible first nodes to move to
  std::vector<std::pair<int, int>> start = this->connections(from);
  // add the destination node to the tree
  int target = this->path_nodes.size();
  int size = target + 1;
  std::vector<std::pair<int, std::vector<std::pair<int, int>>>> backups;
  // std::cout << "Valid ending positions:";
  for(std::pair<int, int> connection : this->connections(to)) {
    // std::cout << " (" << this->path_nodes[connection.first].x << "," << this->path_nodes[connection.first].y << ":" << connection.second << ")";
    backups.push_back(std::pair<int, std::vector<std::pair<int, int>>>(connection.first, this->path_connections[connection.first]));
    this->path_connections[connection.first].push_back(std::pair<int, int>(target, connection.second));
  }
  // std::cout << std::endl;

  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, comp> Q;

  bool finished_with[size];
  int previous[size];
  int distance[size];
  for (int i = 0; i < size; i++) {
    finished_with[i] = false;
    distance[i] = 1 << 30;
    previous[i] = -1;
  }
  distance[size] = 0;
  previous[size] = size;
  // std::cout << "Valid starting positions:";
  for(std::pair<int, int> initial : start) {
    // std::cout << " (" << this->path_nodes[initial.first].x << "," << this->path_nodes[initial.first].y << ":" << initial.second << ")";
    distance[initial.first] = initial.second;
    Q.push(initial);
  }
  // std::cout << std::endl;

  while (!Q.empty()) {
    int current_node = Q.top().first;
    Q.pop();
    if (current_node == target) break;
    if (finished_with[current_node]) continue;
    int connections = this->path_connections[current_node].size();
    // std::cout << "visiting from " << current_node << ":";
    for (int i = 0; i < connections; i++) {
      int connected_node = this->path_connections[current_node][i].first;
      int connected_dist = this->path_connections[current_node][i].second;
      int new_distance = distance[current_node] + connected_dist;
      if (!finished_with[connected_node] && new_distance < distance[connected_node]) {
        // std::cout << " " << connected_node;
        distance[connected_node] = new_distance;
        previous[connected_node] = current_node;
        Q.push(std::pair<int, int>(connected_node, distance[connected_node]));
      }
    }
    // std::cout << std::endl;
    finished_with[current_node] = true;
  }

  for (int i = 0; i < size; i++) {
    // std::cout << "previous[" << i << "] = " << previous[i] << std::endl;
  }

  std::list<Vector<int>> path;
  int node = target;
  while (node != -1) {
    path.push_front(node == target ? to : this->path_nodes[node]);
    node = previous[node];
  }
  path.push_front(from);

  //    std::cout << "Path:";
  //    std::cout << " (" << from.x << "," << from.y << ")";
  //    for (Vector<int> node : path) {
  //        std::cout << " (" << node.x << "," << node.y << ")";
  //    }
  //    std::cout << std::endl << std::endl;

  for (std::pair<int, std::vector<std::pair<int, int>>> backup : backups) {
    this->path_connections[backup.first] = backup.second;
  }

  return path;
}
