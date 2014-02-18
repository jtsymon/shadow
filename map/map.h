/* 
 * File:   map.h
 * Author: jts
 *
 * Created on 11 January 2014, 1:37 PM
 */

#ifndef MAP_H
#define	MAP_H

#include "../misc.h"
#include "visibility_polygon.h"

typedef struct {
    int width, height;
    line_segment_t* segments;
    unsigned char* data;

} map_t;

/**
 * struct containing details about a tile collision,
 * including the side of the tile that was hit, and the pixel position of the collision
 */
typedef struct {
    int x, y;   // position of the tile in the map
    int id;     // id of the tile
    int side;   // side of the tile that was hit
    int pX, pY; // pixel position of the spot on the tile
} map_tile_collision;

enum {
    map_tile_left,
    map_tile_top,
    map_tile_right,
    map_tile_bottom
} map_tile_side;

extern map_t* map_init(int width, int height, unsigned char data[]);
extern map_t* map_open(char* filename);

extern map_tile_collision map_raycast(double angle, double x, double y);
extern void map_shadow(int x, int y);

#endif	/* MAP_H */

