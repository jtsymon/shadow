/* 
 * File:   map_structures.h
 * Author: jts
 *
 * Created on 18 February 2014, 3:29 PM
 */

#ifndef MAP_STRUCTURES_H
#define	MAP_STRUCTURES_H

typedef struct {
    int width, height;
    list_t* segments;
    unsigned char* data;

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
    int x, y;   // position of the tile in the map
    int id;     // id of the tile
    int side;   // side of the tile that was hit
    int pX, pY; // pixel position of the spot on the tile
} map_tile_collision;

#endif	/* MAP_STRUCTURES_H */

