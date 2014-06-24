
#include <stdlib.h>

#include "vector.h"

inline v2d v2i_to_v2d(v2i v) {
    return (v2d) { (double)v.x, (double)v.y };
}

double v2i_dist(v2i v, v2i w) {
    double dx = w.x - v.x;
    double dy = w.y - v.y;
    return sqrt(dx * dx + dy * dy);
}

int64_t v2i_dist_sq(v2i v, v2i w) {
    double dx = w.x - v.x;
    double dy = w.y - v.y;
    return dx * dx + dy * dy;
}

double v2i_mag(v2i v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

int64_t v2i_dot(v2i v, v2i w) {
    return v.x * w.x + v.y * w.y;
}

v2i v2i_scale(double s, v2i v) {
    return (v2i) { (int)(v.x * s), (int)(v.y * s) };
}

v2i v2i_add(v2i v, v2i w) {
    return (v2i) { v.x + w.x, v.y + w.y };
}

v2i v2i_sub(v2i v, v2i w) {
    return (v2i) { v.x - w.x, v.y - w.y };
}

/**
 * True if the point is to the left of the vector
 */
int v2i_side(v2i v, v2i w, v2i p) {
    return sign((w.x - v.x) * (p.y - v.y) - (w.y - v.y) * (p.x - v.x));
}

/**
 * Calculates the minimum distance between a point and a line segment
 * @param v Start of the line segment
 * @param w End of the line segment
 * @param p Point
 * @return The distance
 */
double v2i_dist_line_segment(v2i v, v2i w, v2i p, bool *endpoint) {
    // Return minimum distance between line segment vw and point p
    *endpoint = true;
    double l2 = v2i_dist_sq(v, w);                  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) {
        return v2i_dist(p, v);                      // v == w case
    }
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    double t = v2i_dot(v2i_sub(p, v), v2i_sub(w, v)) / l2;
    if (t < 0.0) {
        return v2i_dist(p, v);                      // Beyond the 'v' end of the segment
    }
    else if (t > 1.0) {
        return v2i_dist(p, w);                      // Beyond the 'w' end of the segment
    }
    v2i projection = v2i_add(v, v2i_scale(t, v2i_sub(w, v)));   // Projection falls on the segment
    *endpoint = false;
    return v2i_dist(p, projection);
}

/**
 * Calculates the angle between two vectors
 */
double v2i_angle(v2i v) {
    return atan((double)v.y / (double)v.x);
}