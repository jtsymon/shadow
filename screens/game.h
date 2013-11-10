#ifndef screen_game_h
#define screen_game_h

#include <pthread.h>
#include <semaphore.h>


#include "../screen.h"
#include "../main.h"

extern void screen_game_create();
extern void screen_game_destroy();
extern void screen_game_show();
extern void screen_game_hide();
extern void screen_game_render();
extern screen_t* screen_game();

struct {
	pthread_t* execution;
	pthread_t* shading;
} GAME_DATA;

#endif
