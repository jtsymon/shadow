/* 
 * File:   entity.h
 * Author: jts
 *
 * Created on 26 April 2014, 12:53 PM
 */

#ifndef ENTITY_H
#define	ENTITY_H

#include <stdio.h>
#include "../math/math.h"
#include "../math/vector.h"
#include "../map/types.h"

typedef struct {
    v2i pos;
    int radius;
} entity_t;

extern entity_t entity_init(int x, int y);
extern void entity_move(entity_t *e, v2i input, map_t *map);

#endif	/* ENTITY_H */

