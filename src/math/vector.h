/* 
 * File:   vector.h
 * Author: jts
 *
 * Created on 25 April 2014, 1:59 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include <math.h>

typedef struct {
    double x, y;
} vec2;

typedef struct {
    int x, y;
} ivec2;

extern double vec_dist(vec2 v, vec2 w);
extern double vec_dist_sq(vec2 v, vec2 w);
extern double vec_dot(vec2 v, vec2 w);
extern vec2 vec_scale(double s, vec2 v);
extern vec2 vec_add(vec2 v, vec2 w);
extern vec2 vec_sub(vec2 v, vec2 w);
extern double dist_line_segment(vec2 v, vec2 w, vec2 p);

extern double ivec_distance(ivec2 v, ivec2 w);
extern double ivec_distance_squared(ivec2 v, ivec2 w);
extern double ivec_dot(ivec2 v, ivec2 w);
extern vec2 ivec_scale(double s, ivec2 v);
extern ivec2 ivec_add(ivec2 v, ivec2 w);
extern ivec2 ivec_sub(ivec2 v, ivec2 w);
extern double idist_line_segment(ivec2 v, ivec2 w, ivec2 p);

#endif	/* VECTOR_H */