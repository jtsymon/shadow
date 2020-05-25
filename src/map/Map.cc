#include "Map.h"

#include <sys/stat.h>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>

#include "../main.h"
#include "../math/vector.h"
#include "../math/math.h"

MapNode<WallConnection> Map::point_read(const std::string &line) {
  char *buf = (char*) line.c_str();
  if (*buf > '9' || *buf < '0') throw Exception("Invalid point encountered: " + line);
  // advance to end of number
  const char *x = buf;
  while (*buf <= '9' && *buf >= '0') ++buf;
  *buf++ = '\0';
  // advance to next number
  while (*buf == ' ' || *buf == '\t') ++buf;
  if (*buf > '9' || *buf < '0') throw Exception("Invalid point encountered: " + line);
  // advance to end of number
  const char *y = buf;
  while (*buf <= '9' && *buf >= '0') ++buf;
  if (*buf == '\n' || *buf == ' ') *buf = '\0';
  else if (*buf) throw Exception("Invalid point encountered: " + line);

  MapNode<WallConnection> point(atoi(x), atoi(y));
  std::cout << "Point: " << point.x << "," << point.y << std::endl;

  return point;
}

MapSegment Map::segment_read(const std::string &line) {
  char *buf = (char*) line.c_str();
  if (*buf > '9' || *buf < '0') throw Exception("Invalid line segment encountered: " + line);
  // advance to end of number
  const char *a = buf;
  while (*buf <= '9' && *buf >= '0') ++buf;
  *buf++ = '\0';
  // advance to next number
  while (*buf == ' ' || *buf == '\t') ++buf;
  if (*buf > '9' || *buf < '0') throw Exception("Invalid line segment encountered: " + line);
  // advance to end of number
  const char *b = buf;
  while (*buf <= '9' && *buf >= '0') ++buf;
  if (*buf == '\n' || *buf == ' ') *buf = '\0';
  else if (*buf) throw Exception("Invalid line segment encountered: " + line);

  MapSegment segment(atoi(a), atoi(b));

  std::cout << "Line Segment: " << segment.a << " (" <<
    this->points[segment.a].x << "," << this->points[segment.a].y <<
    "), " << segment.b << " (" << this->points[segment.b].x << "," <<
    this->points[segment.b].y << ")" << std::endl;

  return segment;
}

std::vector<int> Map::polygon_read(const std::string &line) {
  char *buf = (char*) line.c_str();
  std::vector<int> polygon;
  for (;;) {
    // advance to next number
    while (*buf == ' ' || *buf == '\t') ++buf;
    if (*buf == '\0' || *buf == '\n') break;
    if (*buf > '9' || *buf < '0') throw Exception("Invalid polygon encountered: " + line);
    // advance to end of number
    const char *id = buf;
    while (*buf <= '9' && *buf >= '0') ++buf;
    if (*buf) *buf++ = '\0';
    polygon.push_back(atoi(id));
  }

  printf("Polygon with %ld line segments\n", polygon.size());

  return polygon;
}

Map::Map(const std::string &filename) : mask(Graphics::width, Graphics::height), blur(Graphics::width, Graphics::height) {
  std::ifstream file(filename.c_str(), std::ios::in);
  if (!file.is_open()) {
    throw Exception("Failed to open " + filename);
  }
  std::string line;
  // read points
  while (file.good()) {
    std::getline(file, line);
    if (line[0] == '\0' || line[0] == '\n') {
      break;
    }
    try {
      this->points.push_back(point_read(line));
    } catch (Exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }
  // read line segments
  while (file.good()) {
    std::getline(file, line);
    if (line[0] == '\0' || line[0] == '\n') {
      break;
    }
    try {
      this->segments.push_back(segment_read(line));
    } catch (Exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }
  // read polygons
  while (file.good()) {
    std::getline(file, line);
    if (line[0] == '\0' || line[0] == '\n') {
      break;
    }
    try {
      this->polygons.push_back(polygon_read(line));
    } catch (Exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }
  std::cout << "loaded " << filename << std::endl;

  // populate lists of connections between nodes
  for (const auto& line : this->segments) {
    this->points[line.a].connected.push_back(&this->points[line.b]);
    this->points[line.b].connected.push_back(&this->points[line.a]);
  }

  this->pathfinder = PathFinder(this);
}

#define NoCollision RayCollision(0, 0, INFINITY)

/**
 * Test if a ray cast from p with gradient m passes between s1 and s2
 * @param p     start point
 * @param m     pre-calculated gradient
 * @param c     pre-calculated y-intercept (constant)
 * @param cosa  pre-calculated cosine of angle
 * @param sina  pre-calculated sine of angle
 * @param s1    start of line-segment
 * @param s2    end of line-segment
 * @return      RayCollision object representing the collision with the line segment, if any
 */
RayCollision __ray_intersect(Vector<int> p, double m, double c,
                             double cosa, double sina, Vector<int> s1, Vector<int> s2) {

  int min_y = std::min(s1.y, s2.y);
  int max_y = std::max(s1.y, s2.y);

  // vertical line segment case
  if (s1.x == s2.x) {
    if ((p.x <= s1.x && cosa <= 0) || (p.x > s1.x && cosa > 0)) return NoCollision;
    double ey = c + s1.x * m;
    if (ey >= min_y && ey <= max_y) {
      double dy = ey - p.y;
      double dx = s1.x - p.x;
      double dist = std::sqrt(dy * dy + dx * dx);

      return RayCollision(s1.x, ey, dist);
    }
    return NoCollision;
  }

  int min_x = std::min(s1.x, s2.x);
  int max_x = std::max(s1.x, s2.x);

  // calculate the gradient now that we know it's non-zero
  double sm = (double) (s2.y - s1.y) / (s2.x - s1.x);
  double sc = s1.y - sm * s1.x;

  // ray starts on the line case
  if (float_equal((double) p.y, sm * p.x + sc) && float_equal((double) p.y, m * p.x + c)
      && p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y) {
    // printf("p.y=%d=%d=%d, p.x=%d, min_x=%d, max_x=%d, min_y=%d, max_y=%d\n", p.y, sm * p.x + sc, m * p.x + c, p.x, min_x, max_x, min_y, max_y);

    return RayCollision(p.x, p.y, 0);
  }
  // parallel lines case
  if (float_equal(m, sm)) {
    // check if they're from the same line
    if (!float_equal(c, sc)) return NoCollision;
    if (p.x >= min_x && p.x <= max_x) {
      if (p.y >= min_y && p.y <= max_y) return RayCollision(p.x, p.y, 0);
      if (p.y <= min_y && sina < 0) return RayCollision(p.x, min_y, min_y - p.y);
      if (p.y >= max_y && sina > 0) return RayCollision(p.x, max_y, p.y - max_y);
    }
    if (p.x <= min_x && cosa > 0) {
      if (p.y >= min_y && p.y <= max_y) return RayCollision(min_x, p.y, min_x - p.x);
      if (p.y <= min_y && sina < 0) {
        double dx = min_x - p.x;
        double dy = min_y - p.y;
        double dist = sqrt(dy * dy + dx * dx);

        return RayCollision(min_x, min_y, dist);
      }
      if (p.y >= max_y && sina > 0) {
        double dx = min_x - p.x;
        double dy = p.y - max_y;
        double dist = sqrt(dy * dy + dx * dx);

        return RayCollision(min_x, max_y, dist);
      }
    }
    if (p.x >= max_x && cosa < 0) {
      if (p.y >= min_y && p.y <= max_y) return RayCollision(max_x, p.y, p.x - max_x);
      if (p.y <= min_y && sina < 0) {
        double dx = p.x - max_x;
        double dy = min_y - p.y;
        double dist = sqrt(dy * dy + dx * dx);

        return RayCollision(max_x, min_y, dist);
      }
      if (p.y >= max_y && sina > 0) {
        double dx = p.x - max_x;
        double dy = p.y - max_y;
        double dist = sqrt(dy * dy + dx * dx);

        return RayCollision(max_x, max_y, dist);
      }
    }

    return NoCollision;
  }
  // standard case
  double ex = (sc - c) / (m - sm);
  if (ex < min_x - M_DELTA || ex > max_x + M_DELTA ||
      (ex < p.x && cosa > 0) || (ex > p.x && cosa < 0)) {
    return NoCollision;
  }
  double ey = c + ex * m;
  if (ey < min_y - M_DELTA || ey > max_y + M_DELTA ||
      (ey < p.y && sina > 0) || (ey > p.y && sina < 0)) {
    return NoCollision;
  }
  double dx = ex - p.x;
  double dy = ey - p.y;
  double dist = sqrt(dx * dx + dy * dy);

  return RayCollision(ex, ey, dist);
}

/**
 * Test if a vertical ray cast from p passes between s1 and s2
 * @param p     start point
 * @param sina  pre-calculated sine of angle
 * @param s1    start of line-segment
 * @param s2    end of line-segment
 * @return      RayCollision object representing the collision with the line segment, if any
 */
RayCollision __ray_intersect_v(Vector<int> p, double sina, Vector<int> s1, Vector<int> s2) {
  // parallel lines (vertical line segment) case

  int min_x = std::min(s1.x, s2.x);
  int max_x = std::max(s1.x, s2.x);
  int min_y = std::min(s1.y, s2.y);
  int max_y = std::max(s1.y, s2.y);

  if (s2.x == s1.x) {
    if (p.x != s1.x) return NoCollision;
    if (p.y >= min_y && p.y <= max_y) return RayCollision(p.x, p.y, 0);
    if (p.y <= min_y && sina > 0) return RayCollision(p.x, min_y, min_y - p.y);
    if (p.y >= max_y && sina < 0) return RayCollision(p.x, max_y, p.y - max_y);
    return NoCollision;
  }
  // calculate the gradient now that we know it's non-zero
  double sm = (double) (s2.y - s1.y) / (s2.x - s1.x);
  double sc = s1.y - sm * s1.x;
  // ray starts on the line case
  if (float_equal((double) p.y, sm * p.x + sc) &&
      p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y) {
    // printf("p.y=%d=%d, p.x=%d, min_x=%d, max_x=%d, min_y=%d, max_y=%d\n", p.y, sm * p.x + sc, min_x, max_x, min_y, max_y);

    return RayCollision(p.x, p.y, 0);
  }
  // standard case
  if (p.x >= min_x && p.x <= max_x) {
    double ey = sc + p.x * sm;
    if ((p.y >= ey && sina < 0) || (p.y <= ey && sina > 0)) {

      return RayCollision(p.x, ey, std::abs(p.y - ey));
    }
  }
  return NoCollision;
}

/**
 * Find the closest collision of a ray cast from p with gradient m
 * @param p     start point
 * @param m     pre-calculated gradient
 * @param c     pre-calculated y-intercept (constant)
 * @param cosa  pre-calculated cosine of angle
 * @param sina  pre-calculated sine of angle
 * @return      RayCollision object representing the collision, if any
 */
RayCollision Map::__raycast(Vector<int> p, double m, double c, double cosa, double sina) {
  RayCollision collision = NoCollision;
  for (const auto& wall : this->segments) {

    RayCollision new_collision = __ray_intersect(p, m, c, cosa, sina,
                                                 this->points[wall.a], this->points[wall.b]);
    //            if(new_collision.dist != INFINITY) {
    //                printf("%f, %f, %f\n", new_collision.x, new_collision.y, new_collision.dist);
    //            }
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
  }
  return collision;
}

/**
 * Find the closest collision of a vertical ray cast from p
 * @param p     start point
 * @param sina  pre-calculated sine of angle
 * @return      RayCollision object representing the collision, if any
 */
RayCollision Map::__raycast_v(Vector<int> p, double sina) {
  RayCollision collision = NoCollision;
  for (const auto& wall : this->segments) {

    RayCollision new_collision = __ray_intersect_v(p, sina,
                                                   this->points[wall.a], this->points[wall.b]);
    //            if(new_collision.dist != INFINITY) {
    //                printf("%f, %f, %f\n", new_collision.x, new_collision.y, new_collision.dist);
    //            }
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
  }
  return collision;
}

/**
 * Find the closest collision of a ray cast from p
 * @param p     start point
 * @param angle angle of ray
 * @return      RayCollision object representing the collision, if any
 */
RayCollision Map::raycast(Vector<int> p, double angle) {

  double cosa = cos(angle);
  double sina = -sin(angle);

  if (std::abs(cosa) > M_DELTA) {
    double m = sina / cosa;
    double c = p.y - m * p.x;
    return __raycast(p, m, c, cosa, sina);
  } else {
    return __raycast_v(p, sina);
  }
}

/**
 * Find the closest collision of a ray cast from p, bounded by the edges of the visible map
 * @param p     start point
 * @param angle angle of ray
 * @return      RayCollision object representing the collision, if any
 */
RayCollision Map::shadow_raycast(Vector<int> p, double angle) {

  double cosa = cos(angle);
  double sina = -sin(angle);
  RayCollision collision(0, 0, INFINITY);

  if (std::abs(cosa) > M_DELTA) {
    double m = sina / cosa;
    double c = p.y - m * p.x;
    // check edges of the map
    // top
    RayCollision new_collision = __ray_intersect(p, m, c, cosa, sina,
                                                 Vector<int>(0, 0), Vector<int>(Graphics::width * MAP_SCALE, 0));
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
    // right
    new_collision = __ray_intersect(p, m, c, cosa, sina,
                                    Vector<int>(Graphics::width * MAP_SCALE, 0), Vector<int>(Graphics::width * MAP_SCALE, Graphics::height * MAP_SCALE));
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
    // bottom
    new_collision = __ray_intersect(p, m, c, cosa, sina,
                                    Vector<int>(Graphics::width * MAP_SCALE, Graphics::height * MAP_SCALE), Vector<int>(0, Graphics::height * MAP_SCALE));
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
    // left
    new_collision = __ray_intersect(p, m, c, cosa, sina,
                                    Vector<int>(0, Graphics::height * MAP_SCALE), Vector<int>(0, 0));
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
    new_collision = this->__raycast(p, m, c, cosa, sina);
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
  } else {
    // check edges of the map
    // top
    RayCollision new_collision = __ray_intersect_v(p, sina,
                                                   Vector<int>(0, 0), Vector<int>(Graphics::width * MAP_SCALE, 0));
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
    // right
    new_collision = __ray_intersect_v(p, sina,
                                      Vector<int>(Graphics::width, 0), Vector<int>(Graphics::width * MAP_SCALE, Graphics::height * MAP_SCALE));
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
    // bottom
    new_collision = __ray_intersect_v(p, sina,
                                      Vector<int>(Graphics::width * MAP_SCALE, Graphics::height * MAP_SCALE), Vector<int>(0, Graphics::height * MAP_SCALE));
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
    // left
    new_collision = __ray_intersect_v(p, sina,
                                      Vector<int>(0, Graphics::height * MAP_SCALE), Vector<int>(0, 0));
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
    new_collision = this->__raycast_v(p, sina);
    if (new_collision.dist < collision.dist) {
      collision = new_collision;
    }
  }
  return collision;
}

void Map::shadow(Vector<int> p) {
  if (this->regen_buffers) {
    this->mask = Buffer(Graphics::width, Graphics::height);
    this->blur = Buffer(Graphics::width, Graphics::height);
    this->regen_buffers = false;
  }

  std::set<double> angles;
  for (const auto& point : this->points) {
    double angle = atan2(p.y - point.y, point.x - p.x);
    angles.insert(angle_sanify(angle - SHADOW_DELTA));
    angles.insert(angle_sanify(angle));
    angles.insert(angle_sanify(angle + SHADOW_DELTA));
  }
  // check edges of the screen
  angles.insert(angle_sanify(atan2(p.y, -p.x)));
  angles.insert(angle_sanify(atan2(p.y, Graphics::width * MAP_SCALE - p.x)));
  angles.insert(angle_sanify(atan2(p.y - Graphics::height * MAP_SCALE, Graphics::width * MAP_SCALE - p.x)));
  angles.insert(angle_sanify(atan2(p.y - Graphics::height * MAP_SCALE, -p.x)));

  int size = angles.size() + 3;
  printf("COUNTED %d/%lu ANGLES\n\n\n", size, this->points.size());

  // Draw shadow mask
  mask.begin();

  GLfloat data[size * 2];
  int i = 0;

  data[i++] = Graphics::game_to_gl_x(p.x);
  data[i++] = Graphics::game_to_gl_y(p.y);

  for (const auto angle : angles) {
    RayCollision collision = this->shadow_raycast(p, angle);
    data[i++] = Graphics::game_to_gl_x(collision.x);
    data[i++] = Graphics::game_to_gl_y(collision.y);
  }

  // Finish with the original points to close the geometry.
  data[i++] = data[2];
  data[i++] = data[3];
  data[i++] = data[0];
  data[i++] = data[1];

  // background color
  glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(Graphics::shaders[GRAPHICS_SHADOW_SHADER]);

  glBindVertexArray(Graphics::vertex_array[0]);

  glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
  glVertexAttribPointer(
                        0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
                        2, // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0, // stride
                        NULL // array buffer offset
                        );
  glDrawArrays(GL_TRIANGLE_FAN, 0, size);
  glDisableVertexAttribArray(0);

  glUseProgram(0);

  mask.end();

  blur.begin();

  // background color
  glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLuint shader = Graphics::shaders[GRAPHICS_BLUR_SHADER];

  glUseProgram(shader);

  // Bind our texture in Texture Unit
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mask.getTexture());
  // Set our "renderedTexture" sampler to user Texture Unit 0
  glUniform1i(glGetUniformLocation(shader, "texture"), 0);

  glUniform2f(glGetUniformLocation(shader, "pixel_size"), 1.f / Graphics::width, 1.f / Graphics::height);

  glBindVertexArray(Graphics::vertex_array[0]);

  const GLfloat points[] = {
    -1, 1, 0, 0,
    1, 1, 1, 0,
    1, -1, 1, 1,
    -1, -1, 0, 1
  };

  glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof (GLfloat), points, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
  glVertexAttribPointer(
                        0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
                        4, // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0, // stride
                        (void*) 0 // array buffer offset
                        );
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(0);

  glUseProgram(0);

  blur.end();

  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

  blur.draw(RGBA(128, 128, 128, 255), 0, 0, Graphics::width, Graphics::height);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool Map::can_see(Vector<int> start, Vector<int> end) {
  double dist = start.dist(end);
  double angle = angle_sanify(atan2(start.y - end.y, end.x - start.x));
  double cosa = cos(angle);
  double sina = -sin(angle);
  double m;
  if (std::abs(cosa) > M_DELTA) m = sina / cosa;

  RayCollision collision = (std::abs(cosa) > M_DELTA) ?
    __raycast(start, m, (double)start.y - m * start.x, cosa, sina) :
    __raycast_v(start, sina);

  return (dist <= collision.dist);
}

void Map::draw(Graphics g) {
}

void Map::resize(int width, int height) {
  // don't regenerate buffers until the next frame
  // this avoids us regenerating the buffers too rapidly and overloading the memory allocator
  this->regen_buffers = true;
}
