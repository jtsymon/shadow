/* 
 * File:   vector.h
 * Author: jts
 *
 * Created on 25 April 2014, 1:59 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include <stdint.h>
#include "math.h"

typedef struct {
    int x, y;
} v2i;

typedef struct {
    double x, y;
} v2d;

extern v2d v2i_to_v2d(v2i v);
extern double v2i_dist(v2i v, v2i w);
extern int64_t v2i_dist_sq(v2i v, v2i w);
extern double v2i_mag(v2i v);
extern int64_t v2i_dot(v2i v, v2i w);
extern v2i v2i_scale(double s, v2i v);
extern v2i v2i_add(v2i v, v2i w);
extern v2i v2i_sub(v2i v, v2i w);
extern int v2i_side(v2i a, v2i b, v2i c);
extern double v2i_dist_line_segment(v2i v, v2i w, v2i p, bool *endpoint);
extern double v2i_angle(v2i v);

extern v2i v2d_to_v2i(v2d v);
extern double v2d_dist(v2d v, v2d w);
extern double v2d_dist_sq(v2d v, v2d w);
extern double v2d_mag(v2d v);
extern double v2d_dot(v2d v, v2d w);
extern v2d v2d_scale(double s, v2d v);
extern v2d v2d_add(v2d v, v2d w);
extern v2d v2d_sub(v2d v, v2d w);
extern int v2d_side(v2d a, v2d b, v2d c);
extern double v2d_dist_line_segment(v2d v, v2d w, v2d p);

#endif	/* VECTOR_H */