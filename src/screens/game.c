#include "game.h"

int player_speed = 2;
uint64_t render_time;
char* string_buffer = NULL;

static void create() {
    string_buffer = malloc(100);
    game_data.player = entity_init(320, 320);
    game_data.map = map_open("data/test.map");
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
    v2i input = { 0, 0 };
    if(keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A]) {
        input.x -= player_speed;
    }
    if(keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]) {
        input.x += player_speed;
    }
    if(keys[GLFW_KEY_UP] || keys[GLFW_KEY_W]) {
        input.y -= player_speed;
    }
    if(keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S]) {
        input.y += player_speed;
    }
    if (keys[GLFW_KEY_1]) {
        printf("DEBUG ");
        delay_ms(10);
    }
    if (keys[GLFW_KEY_2]) {
        printf("DEBUG ");
        delay_ms(5);
    }
    
	// background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    entity_move(&game_data.player, input, game_data.map);
    
    // draw the map
    buffer_set_colour((RGBA) { 255, 0, 0, 255 });
    buffer_set_mode(GL_LINES);
    int i;
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
//        ray_collision_t collision = map_raycast_a(game_data.player.pos.x, game_data.player.pos.y, a, game_data.map);
//
//        if(collision.dist == INFINITY) {
//            // printf("no collision\n");
//        } else {
//            glColor3ub(0, 255, 0);
//            glBegin(GL_LINES);
//            glVertex2d(game_to_gl_x(game_data.player.pos.x), game_to_gl_y(game_data.player.pos.y));
//            glVertex2d(game_to_gl_x(collision.x), game_to_gl_y(collision.y));
//            glEnd();
//        }
//    }
    // for(i = 0; i < 40; i++)
    map_shadow(game_data.player.pos);
    
    // draw player
    glColor3ub(128, 128, 128);
    fill_rectangle(game_data.player.pos.x - 5, game_data.player.pos.y - 5, game_data.player.pos.x + 5, game_data.player.pos.y + 5);
    
    sprintf(string_buffer, "Player: %d,%d", game_data.player.pos.x, game_data.player.pos.y);
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

screen_t* screen_game() {
	screen_t* screen = malloc(sizeof(screen_t));
	screen->create          = &create;
    screen->destroy         = &destroy;
    screen->show            = &show;
    screen->hide            = &hide;
    screen->render          = &render;
    screen->key_callback    = &key_callback;
    screen->mouse_callback  = &mouse_callback;
    create();
    return screen;
}
