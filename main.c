#include "main.h"

screen_t* screen;

void set_screen(screen_t* newScreen) {
    if(screen != newScreen) {
        screen->f[screen_HIDE]();
        screen = newScreen;
        screen->f[screen_SHOW]();
    }
}

int setup() {
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
	}
    return initGL();
}

int main(void) {
    
    RENDER.width = 640;
    RENDER.height = 640;
    
    int err = setup();
    if(err == 0) {
        // Set up stuff
        GLOBALS.sleep_time = 1000 / 60;		// 60 fps
        keys = SDL_GetKeyboardState(NULL);
        running = SDL_TRUE;
        GLOBALS.screen_menu = screen_menu();
        GLOBALS.screen_game = screen_game();
        screen = GLOBALS.screen_menu;
        
        while(running) {
            SDL_GetMouseState(&mouse.x, &mouse.y);
            screen->f[screen_RENDER]();
        }
    }
    
    // clean up
	printf("Exiting...\n");
    GLOBALS.screen_menu->f[screen_DESTROY]();
    GLOBALS.screen_game->f[screen_DESTROY]();
    if(RENDER.gl_context) {
        SDL_GL_DeleteContext(RENDER.gl_context);
    }
    if(RENDER.window) {
        SDL_DestroyWindow(RENDER.window); 
    }
    SDL_Quit();
    printf("Bye\n");
    return err;
}
