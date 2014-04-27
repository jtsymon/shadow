/* 
 * File:   vector.h
 * Author: jts
 *
 * Created on 25 April 2014, 1:59 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include "math.h"

typedef struct {
    double x, y;
} vec2;

extern double vec_dist(vec2 v, vec2 w);
extern double vec_dist_sq(vec2 v, vec2 w);
extern double vec_mag(vec2 v);
extern double vec_dot(vec2 v, vec2 w);
extern vec2 vec_scale(double s, vec2 v);
extern vec2 vec_add(vec2 v, vec2 w);
extern vec2 vec_sub(vec2 v, vec2 w);
extern int vec_side(vec2 a, vec2 b, vec2 c);
extern double dist_line_segment(vec2 v, vec2 w, vec2 p);

#endif	/* VECTOR_H */