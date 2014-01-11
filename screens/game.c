#include "game.h"

int player_speed = 2;

void screen_game_create() {
//    game_data.map = map_init(5, 5, (unsigned char[]) {
//        0,   10,  20,  30,  40,
//        50,  60,  70,  80,  90,
//        100, 110, 120, 130, 140,
//        150, 160, 170, 180, 190,
//        200, 210, 220, 230, 240
//    });
//    game_data.map = map_init(10, 10, (unsigned char[]) {
//          0, 255,   0, 255,   0, 255,   0, 255,   0, 255,
//        255,   0, 255,   0, 255,   0, 255,   0, 255,   0,
//          0, 255,   0, 255,   0, 255,   0, 255,   0, 255,
//        255,   0, 255,   0, 255,   0, 255,   0, 255,   0,
//          0, 255,   0, 255,   0, 255,   0, 255,   0, 255,
//        255,   0, 255,   0, 255,   0, 255,   0, 255,   0,
//          0, 255,   0, 255,   0, 255,   0, 255,   0, 255,
//        255,   0, 255,   0, 255,   0, 255,   0, 255,   0,
//          0, 255,   0, 255,   0, 255,   0, 255,   0, 255,
//        255,   0, 255,   0, 255,   0, 255,   0, 255,   0
//    });
    game_data.player.x = 320;
    game_data.player.y = 320;
    game_data.tile_size = 64;
    game_data.map = map_init(10, 10, (unsigned char[]) {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255,   0,   0,   0,   0,   0,   0,   0,   0, 255,
        255,   0,   0,   0,   0,   0, 255,   0,   0, 255,
        255,   0, 255,   0,   0,   0, 255,   0,   0, 255,
        255,   0,   0,   0,   0,   0,   0, 255,   0, 255,
        255,   0,   0,   0,   0,   0,   0,   0,   0, 255,
        255,   0, 255,   0,   0,   0,   0,   0,   0, 255,
        255,   0,   0, 255,   0,   0, 255,   0,   0, 255,
        255,   0,   0,   0,   0,   0,   0,   0,   0, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255
    });
    for(int y = 0; y < game_data.map->height; y++) {
        for(int x = 0; x < game_data.map->width; x++) {
            printf("%d,%d: %d\n", x, y, game_data.map->data[x + game_data.map->width * y]);
        }
    }
}

void screen_game_destroy() {
    printf("Cleaning up game screen...\n");
    free(game_data.map);
}
void screen_game_show() {

}
void screen_game_hide() {

}
void screen_game_render() {
	while(SDL_PollEvent(&RENDER.e)) {
		switch(RENDER.e.type) {
			case SDL_QUIT:
				running = SDL_FALSE;
				break;
			case SDL_KEYUP:
				switch(RENDER.e.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						set_screen(GLOBALS.screen_menu);
						break;
				}
				break;
		}
	}
	keymod = SDL_GetModState();
    int dX = 0, dY = 0;
    if(keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        dX -= player_speed;
    }
    if(keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        dX += player_speed;
    }
    if(keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
        dY -= player_speed;
    }
    if(keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
        dY += player_speed;
    }
    game_data.player.x += dX;
    game_data.player.y += dY;
    
	// background color
	SDL_SetRenderDrawColor(RENDER.renderer, 0, 0, 255, 255);
	// clear the screen
	SDL_RenderClear(RENDER.renderer);
	// draw some text
	int i = 0;
	// for(i = 0; i < 750; i++) {
    SDL_Surface* text_surface = TTF_RenderText_Solid(RENDER.font, "GAME SCREEN", (SDL_Color){0,255,0});
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(RENDER.renderer, text_surface);
    const SDL_Rect bounds = {i * 10, 50 * (i % 10), text_surface->w, text_surface->h};
    SDL_FreeSurface(text_surface);
    SDL_RenderCopy(RENDER.renderer, text_texture, NULL, &bounds);
    SDL_DestroyTexture(text_texture);
	// }
    
    // draw the map
    for(int y = 0; y < game_data.map->height; y++) {
        for(int x = 0; x < game_data.map->width; x++) {
            boxRGBA(RENDER.renderer,
                    x * game_data.tile_size,
                    y * game_data.tile_size,
                    x * game_data.tile_size + game_data.tile_size,
                    y * game_data.tile_size + game_data.tile_size,
                    255 - game_data.map->data[x + game_data.map->width * y],
                    255 - game_data.map->data[x + game_data.map->width * y],
                    255 - game_data.map->data[x + game_data.map->width * y],
                    255);
        }
    }
    
    // printf("%d, %d\n", game_data.player.x, game_data.player.y);
    
    // test raycast
//    map_tile_collision hit = map_raycast(M_PI_4 * 3, (double) game_data.player.x / game_data.tile_size, (double) game_data.player.y / game_data.tile_size);
//    // printf("x=%d, y=%d, pX=%d, pY=%d, side=%d (player: x=%f, y=%f, pX=%d, pY=%d)\n", hit.x, hit.y, hit.pX, hit.pY, hit.side, (double) game_data.player.x / game_data.tile_size, (double) game_data.player.y / game_data.tile_size, game_data.player.x, game_data.player.y);
//    boxRGBA(RENDER.renderer, hit.x * game_data.tile_size - 5, hit.y * game_data.tile_size - 5, hit.x * game_data.tile_size + 5, hit.y * game_data.tile_size + 5, 255, 255, 0, 255);
//    boxRGBA(RENDER.renderer, hit.pX - 5, hit.pY - 5, hit.pX + 5, hit.pY + 5, 0, 255, 0, 255);
    
    for(double a = 0; a < M_PI * 2; a+=0.0002) {
        map_tile_collision hit = map_raycast(a, (double) game_data.player.x / game_data.tile_size, (double) game_data.player.y / game_data.tile_size);
        boxRGBA(RENDER.renderer, hit.pX - 1, hit.pY - 1, hit.pX + 1, hit.pY + 1, 0, 255, 0, 255);
        // pixelRGBA(RENDER.renderer, hit.pX, hit.pY, 0, 255, 0, 255);
    }
    
    // draw player
    boxRGBA(RENDER.renderer, game_data.player.x - 5, game_data.player.y - 5, game_data.player.x + 5, game_data.player.y + 5, 255, 0, 0, 255);

	// draw to the screen
	SDL_RenderPresent(RENDER.renderer);
	

	Uint32 sleep_time = GLOBALS.sleep_time - (SDL_GetTicks() - GLOBALS.last_tick);

	if(sleep_time <= GLOBALS.sleep_time) {
		SDL_Delay(sleep_time);
		// printf("%llu\n", sleep_time);
	} else {
		printf("LAG!\n");
	}

	GLOBALS.last_tick = SDL_GetTicks();
}

void (*screen_game_vtable[])() = { &screen_game_create,
                                   &screen_game_destroy,
                                   &screen_game_show,
                                   &screen_game_hide,
                                   &screen_game_render };
screen_t* screen_game() {
	screen_t* screen = malloc(sizeof(screen_t));
	screen->f = screen_game_vtable;
    screen_game_create();
    return screen;
}
