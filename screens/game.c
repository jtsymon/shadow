#include "game.h"

void screen_game_create() {
	
}
void screen_game_destroy() {

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
						screen = GLOBALS.screen_menu;
						break;
				}
				break;
		}
	}
	keymod = SDL_GetModState();
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
    return screen;
}
