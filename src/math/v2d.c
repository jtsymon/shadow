
#include <stdlib.h>

#include "vector.h"

inline v2i v2d_to_v2i(v2d v) {
    return (v2i) { (int)v.x, (int)v.y };
}

double v2d_dist(v2d v, v2d w) {
    double dx = w.x - v.x;
    double dy = w.y - v.y;
    return sqrt(dx * dx + dy * dy);
}

double v2d_dist_sq(v2d v, v2d w) {
    double dx = w.x - v.x;
    double dy = w.y - v.y;
    return dx * dx + dy * dy;
}

double v2d_mag(v2d v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

double v2d_dot(v2d v, v2d w) {
    return v.x * w.x + v.y * w.y;
}

v2d v2d_scale(double s, v2d v) {
    return (v2d) { v.x * s, v.y * s };
}

v2d v2d_add(v2d v, v2d w) {
    return (v2d) { v.x + w.x, v.y + w.y };
}

v2d v2d_sub(v2d v, v2d w) {
    return (v2d) { v.x - w.x, v.y - w.y };
}

/**
 * True if the point is to the left of the vector
 */
int v2d_side(v2d v, v2d w, v2d p) {
    return sign((w.x - v.x) * (p.y - v.y) - (w.y - v.y) * (p.x - v.x));
}

/**
 * Calculates the minimum distance between a point and a line segment
 * @param v Start of the line segment
 * @param w End of the line segment
 * @param p Point
 * @return The distance
 */
double v2d_dist_line_segment(v2d v, v2d w, v2d p) {
    // Return minimum distance between line segment vw and point p
    double l2 = v2d_dist_sq(v, w);                          // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) return v2d_dist(p, v);                        // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    double t = v2d_dot(v2d_sub(p, v), v2d_sub(w, v)) / l2;
    if (t < 0.0) return v2d_dist(p, v);                          // Beyond the 'v' end of the segment
    else if (t > 1.0) return v2d_dist(p, w);                     // Beyond the 'w' end of the segment
    v2d projection = v2d_add(v, v2d_scale(t, v2d_sub(w, v))); // Projection falls on the segment
    return v2d_dist(p, projection);
}