/* 
 * File:   game.h
 * Author: jts
 *
 * Created on 10 Nov 2013
 */

#ifndef SCREEN_GAME_H
#define SCREEN_GAME_H

#include <pthread.h>
#include <semaphore.h>

#include "../misc.h"
#include "../screen.h"
#include "../map/map.h"
#include "../main.h"
#include "../engine/timer.h"
#include "../math/vector.h"

extern screen_t* screen_game();

struct {
    pthread_t* execution;
	pthread_t* shading;
    map_t *map;
    int tile_size;
    ivec2 player;
} game_data;

#endif /* SCREEN_GAME_H */
