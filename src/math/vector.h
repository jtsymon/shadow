#pragma once

#include <stdint.h>
#include "math.h"

template <class T>
class Vector {
 public:
  T x, y;

 Vector(T x, T y) : x(x), y(y) {
  }

  T dist_sq(Vector<T> v) {
    double dx = x - v.x;
    double dy = y - v.y;
    return dx * dx + dy * dy;
  }

  double dist(Vector<T> v) {
    return sqrt(dist_sq(v));
  }

  T magnitude_sq() {
    return x * x + y * y;
  }

  double magnitude() {
    return sqrt(magnitude_sq());
  }

  T dot(Vector v) {
    return x * v.x + y * v.y;
  }

  template<class F>
  Vector<T> operator*(F factor) {
    return Vector<T>(x * factor, y * factor);
  }

  template<class F>
  void operator*=(F factor) {
    x *= factor;
    y *= factor;
  }

  Vector<T> operator+(Vector<T> v) {
    return Vector<T>(x + v.x, y + v.y);
  }

  void operator+=(Vector<T> v) {
    x += v.x;
    y += v.y;
  }

  Vector<T> operator-(Vector<T> v) {
    return Vector<T>(x - v.x, y - v.y);
  }

  void operator-=(Vector<T> v) {
    x -= v.x;
    y -= v.y;
  }

  Vector<T> normalise() {
    return *this * (1.0 / std::abs(this->magnitude()));
  }

  template<class F>
  Vector<T> normalise(F factor) {
    return *this * (((double) factor) / std::abs(magnitude()));
  }

  Vector<int> toInt() {
    return Vector<int>((int) x, y);
  }

  Vector<double> toDouble() {
    return Vector<double>((double) x, (double) y);
  }

  double angle() {
    return std::atan2((double)y, (double)x);
  }

  bool operator==(const Vector<T> &other) const {
    return (this->x == other.x && this->y == other.y);
  }
};

template <class T>
class LineSegment {
 public:
  Vector<T> v1, v2;

 LineSegment(Vector<T> v1, Vector<T> v2) : v1(v1), v2(v2) {
  }

  int side(Vector<T> v) {
    return sign((v2.x - v1.x) * (v.y - v1.y) - (v2.y - v1.y) * (v.x - v1.x));
  }

  double dist(Vector<T> p, bool &endpoint) {
    // Return minimum distance between line segment (v1, v2) and point p
    endpoint = true;
    double l2 = v1.dist_sq(v2); // i.e. |v2-v1|^2 -  avoid a sqrt
    if (l2 == 0.0) {
      return p.dist(v1); // v1 == v2 case
    }
    // Consider the line extending the segment, parameterized as v1 + t (v2 - v1).
    // We find projection of point p onto the line.
    // It falls where t = [(p - v1) . (v2 - v1)] / |v2 - v1|^2
    double t = (p - v1).dot(v2 - v1) / l2;
    if (t < 0.0) {
      return p.dist(v1); // Beyond the 'v1' end of the segment
    } else if (t > 1.0) {
      return p.dist(v2); // Beyond the 'v2' end of the segment
    }
    Vector<double> projection = v1.toDouble() + ((v2 - v1).toDouble() * t); // Projection falls on the segment
    endpoint = false;
    return p.toDouble().dist(projection);
  }

  bool on_line(Vector<T> p) {
    double l2 = v1.dist_sq(v2); // i.e. |v2-v1|^2 -  avoid a sqrt
    if (l2 == 0.0) {
      return false; // v1 == v2 case
    }
    // Consider the line extending the segment, parameterized as v1 + t (v2 - v1).
    // We find projection of point p onto the line.
    // It falls where t = [(p - v1) . (v2 - v1)] / |v2 - v1|^2
    double t = (p - v1).dot(v2 - v1) / l2;
    if (t >= 0.0 && t <= 1.0) {
      return true;
    }
    return false;
  }

  bool intersects(LineSegment<T> ls) {
    // NOTE: returns intersections with lines past the end of the segment
    return side(ls.v1) != side(ls.v2);
  }

  Vector<double> normal(int side) {
    if (side == 1) {
      return Vector<double>((double) (v1.y - v2.y), (double) (v2.x - v1.x));
    } else {
      return Vector<double>((double) (v2.y - v1.y), (double) (v1.x - v2.x));
    }
  }
};
