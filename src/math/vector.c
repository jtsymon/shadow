
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
 * Calculates the minimum distance between a point and a line segment
 * @param v Start of the line segment
 * @param w End of the line segment
 * @param p Point
 * @return The distance
 */
static inline double __dist_line_segment(vec2 v, vec2 w, vec2 p) {
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

double dist_line_segment(vec2 v, vec2 w, vec2 p) {
    return __dist_line_segment(v, w, p);
}

double idist_line_segment(ivec2 v, ivec2 w, ivec2 p) {
    return __dist_line_segment((vec2) { (double) v.x, (double) v.y },
            (vec2) { (double) w.x, (double) w.y },
            (vec2) { (double) p.x, (double) p.y });
}

double ivec_dist(ivec2 v, ivec2 w) {
    double dx = w.x - v.x;
    double dy = w.y - v.y;
    return sqrt(dx * dx + dy * dy);
}

double ivec_dist_sq(ivec2 v, ivec2 w) {
    double dx = w.x - v.x;
    double dy = w.y - v.y;
    return dx * dx + dy * dy;
}

double ivec_dot(ivec2 v, ivec2 w) {
    return v.x * w.x + v.y * w.y;
}

vec2 ivec_scale(double s, ivec2 v) {
    return (vec2) { v.x * s, v.y * s };
}

ivec2 ivec_add(ivec2 v, ivec2 w) {
    return (ivec2) { v.x + w.x, v.y + w.y };
}

ivec2 ivec_sub(ivec2 v, ivec2 w) {
    return (ivec2) { v.x - w.x, v.y - w.y };
}