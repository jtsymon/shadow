#include "menu.h"

void screen_menu_create() {
	
}
void screen_menu_destroy() {

}
void screen_menu_show() {

}
void screen_menu_hide() {

}
void screen_menu_render() {
	while(SDL_PollEvent(&RENDER.e)) {
		switch(RENDER.e.type) {
			case SDL_QUIT:
				running = SDL_FALSE;
				break;
			case SDL_MOUSEBUTTONDOWN:
				screen = GLOBALS.screen_game;
				break;
			case SDL_KEYUP:
				switch(RENDER.e.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						running = SDL_FALSE;
						break;
				}
				break;
		}
	}
	keymod = SDL_GetModState();
	if(keys[SDL_SCANCODE_A]) {
		printf("DEBUG ");
		SDL_Delay(10);
	}
	if(keys[SDL_SCANCODE_S]) {
		printf("DEBUG ");
		SDL_Delay(5);
	}
	// red background color
	SDL_SetRenderDrawColor(RENDER.renderer, 255, 0, 0, 255);
	// clear the screen
	SDL_RenderClear(RENDER.renderer);
	// draw some text
	int i = 0;
	// for(i = 0; i < 750; i++) {
		SDL_Surface* text_surface = TTF_RenderText_Solid(RENDER.font, "Testing 123", (SDL_Color){0,255,0});
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
		printf("%llu\n", sleep_time);
	} else {
		printf("LAG!\n");
	}

	GLOBALS.last_tick = SDL_GetTicks();
}

void (*screen_menu_vtable[])() = {&screen_menu_create, &screen_menu_destroy, &screen_menu_show, &screen_menu_hide, &screen_menu_render};
screen_t* screen_menu() {
	screen_t* screen = malloc(sizeof(screen_t));
	screen->f = screen_menu_vtable;
}
