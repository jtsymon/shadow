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

/*
 * Map Format:
 * 
 * List of points:
 * x,y
 * x,y
 * x,y
 * 
 * List of line formed from points:
 * pointid:pointid
 * pointid:pointid
 * pointid:pointid
 * 
 * List of polygons:
 * segmentid,segmentid,segmentid:texture
 * segmentid,segmentid,segmentid,segmentid:texture
 */

extern map_t* map_open(char* filename);

//extern map_tile_collision map_raycast(double angle, double x, double y);
extern ray_collision_t map_raycast_a(double x, double y, double angle, map_t *map);
extern void map_shadow(double x, double y);

#endif	/* MAP_H */

