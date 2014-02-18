/* 
 * File:   map.h
 * Author: jts
 *
 * Created on 11 January 2014, 1:37 PM
 */

#ifndef MAP_H
#define	MAP_H

#include "../misc.h"
#include "../list.h"
#include "map_structures.h"
#include "visibility.h"

extern map_t* map_init(int width, int height, unsigned char data[]);
extern map_t* map_open(char* filename);

extern map_tile_collision map_raycast(double angle, double x, double y);
extern void map_shadow(int x, int y);

#endif	/* MAP_H */

