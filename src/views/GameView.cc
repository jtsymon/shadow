#include "GameView.h"
#include "../main.h"
#include "../graphics/BitmapFont.h"
#include "../engine/Timing.h"
#include <string>
#include <iostream>

int player_speed = 2;
uint64_t render_time;

GameView::GameView() : player{Entity(320, 320)}, map{Map("data/simple.map")} {
    
}

GameView::~GameView() {
    printf("Cleaning up game screen...\n");
}
void GameView::show() {
    
}
void GameView::hide() {

}

static bool update = true;
void GameView::render() {
    
    Graphics g = Graphics::get();
    
    render_time = ticks_ms();
    
    if(!update) {
        delay_ms(16);
        return;
    }
    Vector<int> input(0, 0);
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
    
    this->player.move(input, this->map);
    
    // draw the map
    Batch map_batch(GL_LINES, RGBA(255, 0, 0, 255), Graphics::shaders[GRAPHICS_COLOUR_SHADER], map.segments.size() * 2);
    for(MapSegment segment : map.segments) {
        const GLfloat points[] = {
            game_to_gl_x(map.points[segment.a].x),
            game_to_gl_y(map.points[segment.a].y),
            game_to_gl_x(map.points[segment.b].x),
            game_to_gl_y(map.points[segment.b].y)
        };
        map_batch.add(2, points);
    }
    // glLineWidth(5.0);
    g.draw(map_batch);
    
    // g.fill_rectangle(RGBA(0, 255, 0, 255), 100, 300, 200, 400);
    
    // this->map.shadow(this->player.pos);
    
    Batch visibility_lines(GL_LINES, RGBA(255, 255, 0, 64), Graphics::shaders[GRAPHICS_COLOUR_SHADER], 10248);
    for (MapNode start : this->map.path_nodes) {
        for(MapNode *end : start.connected) {
            const GLfloat points[] = {
                game_to_gl_x(start.x),
                game_to_gl_y(start.y),
                game_to_gl_x(end->x),
                game_to_gl_y(end->y)
            };
            visibility_lines.add(2, points);
        }
    }
    g.draw(visibility_lines);
    
    // draw player
    g.fill_rectangle(
            RGBA(128, 128, 128, 255),
            this->player.pos.x - 5, this->player.pos.y - 5,
            this->player.pos.x + 5, this->player.pos.y + 5
    );
    
    std::string message = "Player: " + std::to_string(this->player.pos.x) + "," + std::to_string(this->player.pos.y);
    BitmapFont::standard()->drawText(350, 10, message, RGBA(0, 255, 0, 255));
    
    // draw fps
    message = "Render time: " + std::to_string(ticks_ms() - render_time) + "ms";
    BitmapFont::standard()->drawText(10, 10, message, RGBA(0, 255, 0, 255));
    // std::cout << message << std::endl;
}

void GameView::key_callback(int key, int scancode, int action, int mods) {
    if(action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
                set_view(menu_view);
				break;
            case GLFW_KEY_P:
                update = !update;
                break;
		}
    }
}

void GameView::mouse_callback(int button, int action, int mods) {
    
}