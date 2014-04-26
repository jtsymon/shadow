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

typedef struct {
    int x, y;
} ivec2;

extern vec2 ivec_to_vec(ivec2 v);

extern double vec_dist(vec2 v, vec2 w);
extern double ivec_dist(ivec2 v, ivec2 w);

extern double vec_dist_sq(vec2 v, vec2 w);
extern double ivec_dist_sq(ivec2 v, ivec2 w);

extern double vec_mag(vec2 v);
extern double ivec_mag(ivec2 v);

extern double vec_dot(vec2 v, vec2 w);
extern double ivec_dot(ivec2 v, ivec2 w);

extern vec2 vec_scale(double s, vec2 v);
extern vec2 ivec_scale(double s, ivec2 v);

extern vec2 vec_add(vec2 v, vec2 w);
extern ivec2 ivec_add(ivec2 v, ivec2 w);

extern vec2 vec_sub(vec2 v, vec2 w);
extern ivec2 ivec_sub(ivec2 v, ivec2 w);

extern int vec_side(vec2 a, vec2 b, vec2 c);
extern int ivec_side(ivec2 a, ivec2 b, ivec2 c);

extern double dist_line_segment(vec2 v, vec2 w, vec2 p);
extern double idist_line_segment(ivec2 v, ivec2 w, ivec2 p);

#endif	/* VECTOR_H */