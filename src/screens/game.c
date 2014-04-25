#include "game.h"

int player_speed = 2;
uint64_t render_time;
char* string_buffer = NULL;

static void create() {
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

static void destroy() {
    printf("Cleaning up game screen...\n");
    if(string_buffer != NULL) {
        free(string_buffer);
    }
    if(game_data.map != NULL) {
        free(game_data.map);
    }
}
static void show() {
    
}
static void hide() {

}

static bool update = true;
static void render() {
    
    render_time = ticks_ms();
    
    if(!update) {
        delay_ms(16);
        return;
    }
    int dX = 0, dY = 0;
    if(keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A]) {
        dX -= player_speed;
    }
    if(keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]) {
        dX += player_speed;
    }
    if(keys[GLFW_KEY_UP] || keys[GLFW_KEY_W]) {
        dY -= player_speed;
    }
    if(keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S]) {
        dY += player_speed;
    }
    if (keys[GLFW_KEY_1]) {
        printf("DEBUG ");
        delay_ms(10);
    }
    if (keys[GLFW_KEY_2]) {
        printf("DEBUG ");
        delay_ms(5);
    }
    int i;
    {
#define collide_dist 10
        ivec2 pos = { game_data.player.x + dX, game_data.player.y + dY };
        ivec2 posx = { game_data.player.x + dX, game_data.player.y };
        ivec2 posy = { game_data.player.x, game_data.player.y + dY };
        
        int move = 0;

        for(i = 0; i < game_data.map->n_segments; i++) {
            ivec2 v = game_data.map->points[game_data.map->segments[i].a];
            ivec2 w = game_data.map->points[game_data.map->segments[i].b];
            if(idist_line_segment(v, w, pos) < collide_dist) {
                if(idist_line_segment(v, w, posx) < collide_dist) move |= 1;
                if(idist_line_segment(v, w, posy) < collide_dist) move |= 2;
                if((move & 3) == 3) break;
            }
        }
#undef collide_dist
        
        printf("%d %d %d\n", move, dX, dY);
        if((move & 1) == 0) game_data.player.x += dX;
        if((move & 2) == 0) game_data.player.y += dY;
    }
    
	// background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // draw the map
    buffer_set_colour((RGBA) { 255, 0, 0, 255 });
    buffer_set_mode(GL_LINES);
    for(i = 0; i < game_data.map->n_segments; i++) {
        buffer_add_n(2, (GLfloat[]) {
            game_to_gl_x(game_data.map->points[game_data.map->segments[i].a].x),
                    game_to_gl_y(game_data.map->points[game_data.map->segments[i].a].y),
            game_to_gl_x(game_data.map->points[game_data.map->segments[i].b].x),
                    game_to_gl_y(game_data.map->points[game_data.map->segments[i].b].y)
        });
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
    // for(i = 0; i < 40; i++)
    map_shadow(game_data.player.x, game_data.player.y);
    
    // draw player
    glColor3ub(128, 128, 128);
    fill_rectangle(game_data.player.x - 5, game_data.player.y - 5, game_data.player.x + 5, game_data.player.y + 5);
    
    sprintf(string_buffer, "Player: %d,%d (%-1.2f,%-1.2f)", game_data.player.x, game_data.player.y,
            (float) game_data.player.x / game_data.tile_size, (float) game_data.player.y / game_data.tile_size);
    draw_text(350, 10, string_buffer, (RGBA) {0, 255, 0, 255});
    
    // draw fps
    sprintf(string_buffer, "Render time: %dms", (ticks_ms() - render_time));
    draw_text(10, 10, string_buffer, (RGBA) {0, 255, 0, 255});
}

static void key_callback(int key, int scancode, int action, int mods) {
    if(action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
                set_screen(GLOBALS.screen_menu);
				break;
            case GLFW_KEY_P:
                update = !update;
                break;
		}
    }
}

static void mouse_callback(int button, int action, int mods) {
    
}

static void (*vtable[])() = {
    &create,
    &destroy,
    &show,
    &hide,
    &render,
    &key_callback,
    &mouse_callback
};
screen_t* screen_game() {
	screen_t* screen = malloc(sizeof(screen_t));
	screen->f = vtable;
    create();
    return screen;
}
