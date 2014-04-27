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
#include "types.h"
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

extern ray_collision_t __ray_intersect(v2i p, double m, double c, double cosa, double sina, v2i s1, v2i s2);
extern ray_collision_t __ray_intersect_v(v2i p, double sina, v2i s1, v2i s2);
extern ray_collision_t __raycast(v2i p, double m, double c, double cosa, double sina, map_t *map);
extern ray_collision_t __raycast_v(v2i p, double sina, map_t *map);
extern ray_collision_t map_raycast_a(v2i p, double angle, map_t *map);

#endif	/* MAP_H */

