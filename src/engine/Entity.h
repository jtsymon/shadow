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
#include "../map/Map.h"

class Entity {
public:
    Vector<int> pos;
    int radius;

    Entity(int x, int y);
    void move(Vector<int> motion, Map map);
};



#endif	/* ENTITY_H */

