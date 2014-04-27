
#include "vector.h"

double vec_dist(vec2 v, vec2 w) {
    double dx = w.x - v.x;
    double dy = w.y - v.y;
    return sqrt(dx * dx + dy * dy);
}

double vec_dist_sq(vec2 v, vec2 w) {
    double dx = w.x - v.x;
    double dy = w.y - v.y;
    return dx * dx + dy * dy;
}

double vec_mag(vec2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

double vec_dot(vec2 v, vec2 w) {
    return v.x * w.x + v.y * w.y;
}

vec2 vec_scale(double s, vec2 v) {
    return (vec2) { v.x * s, v.y * s };
}

vec2 vec_add(vec2 v, vec2 w) {
    return (vec2) { v.x + w.x, v.y + w.y };
}

vec2 vec_sub(vec2 v, vec2 w) {
    return (vec2) { v.x - w.x, v.y - w.y };
}

/**
 * True if the point is to the left of the vector
 */
int vec_side(vec2 v, vec2 w, vec2 p) {
    return sign((w.x - v.x) * (p.y - v.y) - (w.y - v.y) * (p.x - v.x));
}

/**
 * Calculates the minimum distance between a point and a line segment
 * @param v Start of the line segment
 * @param w End of the line segment
 * @param p Point
 * @return The distance
 */
double dist_line_segment(vec2 v, vec2 w, vec2 p) {
    // Return minimum distance between line segment vw and point p
    double l2 = vec_dist_sq(v, w);                          // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) return vec_dist(p, v);                        // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    double t = vec_dot(vec_sub(p, v), vec_sub(w, v)) / l2;
    if (t < 0.0) return vec_dist(p, v);                          // Beyond the 'v' end of the segment
    else if (t > 1.0) return vec_dist(p, w);                     // Beyond the 'w' end of the segment
    vec2 projection = vec_add(v, vec_scale(t, vec_sub(w, v))); // Projection falls on the segment
    return vec_dist(p, projection);
}