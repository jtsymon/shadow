#include "main.h"

int main(void) {

        // Initialize SDL.
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                return 1;
	}
	if(TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		return 2;
	}

        // Create the window where we will draw.
        RENDER.window = SDL_CreateWindow("SDL_RenderClear",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        512, 512,
                        0);

        // We must call SDL_CreateRenderer in order for draw calls to affect this window.
        RENDER.renderer = SDL_CreateRenderer(RENDER.window, -1, 0);

	RENDER.font = TTF_OpenFont("fonts/Vera.ttf", 16);
	if(!RENDER.font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		return 2;
	}

	// Set up stuff
	GLOBALS.sleep_time = 1000 / 60;		// 60 fps
	keys = SDL_GetKeyboardState(NULL);
	running = SDL_TRUE;
	GLOBALS.screen_menu = screen_menu();
	GLOBALS.screen_game = screen_game();
	screen = GLOBALS.screen_menu;

	while(running) {
		screen->f[screen_RENDER]();
	}

        // clean up
	printf("Exiting...\n");
	if(RENDER.font) {
		TTF_CloseFont(RENDER.font);
	}
	TTF_Quit();
        SDL_Quit();
        return 0;
}
