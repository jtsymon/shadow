#pragma once

#include "../misc.h"
#include "../graphics/Graphics.h"
#include "../graphics/Shaders.h"
#include "../graphics/Buffer.h"
#include "../math/vector.h"
#include "types.h"
#include "PathFinder.h"
#include <vector>
#include <string>

#define MAP_SCALE 100.f

/*
 * Map Format:
 *
 * List of points:
 * x,y
 * x,y
 * x,y
 *
 * List of line formed from points:
 * pointid:pointid
 * pointid:pointid
 * pointid:pointid
 *
 * List of polygons:
 * segmentid,segmentid,segmentid:texture
 * segmentid,segmentid,segmentid,segmentid:texture
 */

struct MapSegment {
public:
  // indices into the point array
  int a, b;

MapSegment(int a, int b) : a(a), b(b) {
}
};

struct RayCollision {
public:
  int x, y;
  double dist;

RayCollision(int x, int y, double dist) : x(x), y(y), dist(dist) {
}
};

class Map {

  friend class PathFinder;

  MapNode<WallConnection> point_read(const std::string &line);
  MapSegment segment_read(const std::string &line);
  std::vector<int> polygon_read(const std::string &line);
  RayCollision __raycast(Vector<int> p, double m, double c, double cosa, double sina);
  RayCollision __raycast_v(Vector<int> p, double sina);
  RayCollision shadow_raycast(Vector<int> p, double angle);
  bool can_see(Vector<int> start, Vector<int> end);
  float scale;

  Buffer mask;
  Buffer blur;

 public:
  // unique endpoints of line_segments
  std::vector<WallConnection> points;
  // line segments made up from points
  std::vector<MapSegment> segments;
  // combinations of line segments, used for drawing wall textures
  std::vector<std::vector<int>> polygons;

  PathFinder pathfinder;

  Map(const std::string &name);
  RayCollision raycast(Vector<int> p, double angle);
  void shadow(Vector<int> p);
  void draw(Graphics g);

  // static ShadowMask shadow_mask;
};
