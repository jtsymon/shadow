#ifndef TYPES_H
#define	TYPES_H

#include <vector>
#include "../math/vector.h"

template <class T>
struct MapNode : public Vector<int> {
 public:
  std::vector<T> connected;
 MapNode(int x, int y) : Vector<int>::Vector(x, y) { }
 MapNode(Vector<int> vec) : Vector<int>::Vector(vec.x, vec.y) { }
};

struct WallConnection : public MapNode<WallConnection*> {
  using MapNode<WallConnection*>::MapNode;
};

#endif	/* TYPES_H */
