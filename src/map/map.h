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
#include "shadow.h"

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

extern ray_collision_t __ray_intersect(double x, double y, double m, double c, double cosa, double sina,
        double x1, double y1, double x2, double y2);
extern ray_collision_t __ray_intersect_v(double x, double y, double sina,
        double x1, double y1, double x2, double y2);
extern ray_collision_t __raycast(double x, double y, double m, double c, double cosa, double sina, map_t *map);
extern ray_collision_t __raycast_v(double x, double y, double sina, map_t *map);
extern ray_collision_t map_raycast_a(double x, double y, double angle, map_t *map);

#endif	/* MAP_H */

