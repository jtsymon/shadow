#include <GL/glew.h>

#include "game.h"

int player_speed = 2;
int render_time;
char* string_buffer = NULL;

void screen_game_create() {
    string_buffer = malloc(100);
    game_data.player.x = 320;
    game_data.player.y = 320;
    game_data.tile_size = 64;
//    game_data.map = map_init(10, 10, (unsigned char[]) {
//        255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//        255,   0,   0,   0,   0,   0,   0,   0,   0, 255,
//        255,   0,   0,   0,   0,   0, 255,   0,   0, 255,
//        255,   0, 255,   0,   0,   0, 255,   0,   0, 255,
//        255,   0,   0,   0,   0,   0,   0, 255,   0, 255,
//        255,   0,   0,   0,   0,   0,   0,   0,   0, 255,
//        255,   0, 255,   0,   0,   0,   0,   0,   0, 255,
//        255,   0,   0, 255,   0,   0, 255,   0,   0, 255,
//        255,   0,   0,   0,   0,   0,   0,   0,   0, 255,
//        255, 255, 255, 255, 255, 255, 255, 255, 255, 255
//    });
    game_data.map = map_open("data/test.map");
//    for(int y = 0; y < game_data.map->height; y++) {
//        for(int x = 0; x < game_data.map->width; x++) {
//            printf("%d,%d: %d\n", x, y, game_data.map->data[x + game_data.map->width * y]);
//        }
//    }
}

void screen_game_destroy() {
    printf("Cleaning up game screen...\n");
    if(string_buffer != NULL) {
        free(string_buffer);
    }
    if(game_data.map != NULL) {
        free(game_data.map);
    }
}
void screen_game_show() {
    
}
void screen_game_hide() {

}

static bool update = true;
void screen_game_render() {
    
    render_time = SDL_GetTicks();
    
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
                    case SDL_SCANCODE_P:
                        update = !update;
                        break;
				}
				break;
		}
	}
    if(!update) {
        SDL_Delay(16);
        return;
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // draw the map
    int i;
    for(i = 0; i < game_data.map->n_segments; i++) {
        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2d(game_to_gl_x(game_data.map->points[game_data.map->segments[i].a].x),
                game_to_gl_y(game_data.map->points[game_data.map->segments[i].a].y));
        glVertex2d(game_to_gl_x(game_data.map->points[game_data.map->segments[i].b].x),
                game_to_gl_y(game_data.map->points[game_data.map->segments[i].b].y));
        glEnd();
    }
    
//    double a;
//    for(a = 0; a < M_PI; a += M_PI_4) {
//
//        ray_collision_t collision = map_raycast_a(game_data.player.x, game_data.player.y, a, game_data.map);
//
//        if(collision.dist == INFINITY) {
//            // printf("no collision\n");
//        } else {
//            glColor3ub(0, 255, 0);
//            glBegin(GL_LINES);
//            glVertex2d(game_to_gl_x(game_data.player.x), game_to_gl_y(game_data.player.y));
//            glVertex2d(game_to_gl_x(collision.x), game_to_gl_y(collision.y));
//            glEnd();
//        }
//    }
    
    map_shadow(game_data.player.x, game_data.player.y);
    
//    for(int y = 0; y < game_data.map->height; y++) {
//        for(int x = 0; x < game_data.map->width; x++) {
//            glColor3ub(255 - game_data.map->data[x + game_data.map->width * y],
//                    255 - game_data.map->data[x + game_data.map->width * y],
//                    255 - game_data.map->data[x + game_data.map->width * y]);
//            fill_rectangle(x * game_data.tile_size,
//                    y * game_data.tile_size,
//                    x * game_data.tile_size + game_data.tile_size,
//                    y * game_data.tile_size + game_data.tile_size);
//        }
//    }
    
    // printf("%d, %d\n", game_data.player.x, game_data.player.y);
    
//    for(double a = 0; a < M_PI * 2; a+=0.0002) {
//        map_tile_collision hit = map_raycast(a, (double) game_data.player.x / game_data.tile_size, (double) game_data.player.y / game_data.tile_size);
//        glColor3ub(0, 255, 0);
//        fill_rectangle(hit.pX - 1, hit.pY - 1, hit.pX + 1, hit.pY + 1);
//        glColor3ub(255, 0, 255);
//        fill_rectangle((hit.x + 0.5) * game_data.tile_size - 5, (hit.y + 0.5) * game_data.tile_size - 5,
//                                (hit.x + 0.5) * game_data.tile_size + 5, (hit.y + 0.5) * game_data.tile_size + 5);
//        // pixelRGBA(RENDER.renderer, hit.pX, hit.pY, 0, 255, 0, 255);
//    }
//    
//    // shadow
//    map_shadow(game_data.player.x, game_data.player.y);
    
    // draw player
    glColor3ub(128, 128, 128);
    fill_rectangle(game_data.player.x - 5, game_data.player.y - 5, game_data.player.x + 5, game_data.player.y + 5);
    
    glColor3ub(0, 255, 0);
    sprintf(string_buffer, "Player: %d,%d (%-1.2f,%-1.2f)", game_data.player.x, game_data.player.y,
            (float) game_data.player.x / game_data.tile_size, (float) game_data.player.y / game_data.tile_size);
    draw_text(350, 10, string_buffer);
    
    // draw fps
    sprintf(string_buffer, "Render time: %dms", (SDL_GetTicks() - render_time));
    draw_text(10, 10, string_buffer);

    SDL_GL_SwapWindow(RENDER.window);
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
