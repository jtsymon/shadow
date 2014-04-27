/* 
 * File:   map_structures.h
 * Author: jts
 *
 * Created on 18 February 2014, 3:29 PM
 */

#ifndef MAP_STRUCTURES_H
#define	MAP_STRUCTURES_H

#include "../list.h"
#include "../math/vector.h"

typedef struct {
    int a, b;
} line_segment_t;

typedef struct {
    // texture_t *texture;
    int n_segments;
    int *segment_ids;
} polygon_t;

typedef struct {
    // unique endpoints of line_segments
    int             n_points;
    vec2             *points;
    // line segments made up from points
    int             n_segments;
    line_segment_t  *segments;
    // combinations of line segments, used for drawing wall textures
    list_t          *polygons;
} map_t;


enum {
    map_tile_left,
    map_tile_top,
    map_tile_right,
    map_tile_bottom
} map_tile_side;

/**
 * struct containing details about a tile collision,
 * including the side of the tile that was hit, and the pixel position of the collision
 */
typedef struct {
    double x, y;
    double dist;
} ray_collision_t;

#endif	/* MAP_STRUCTURES_H */

