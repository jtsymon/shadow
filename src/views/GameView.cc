#include "GameView.h"
#include "../main.h"
#include "../graphics/BitmapFont.h"
#include "../engine/Timing.h"
#include <string>
#include <iostream>

int player_speed = 2 * MAP_SCALE;
uint64_t render_time;
Vector<int> pathfind_to(150, 150);

GameView::GameView() : map{new Map("data/test.map")}, player{Entity(Vector<int>(10000, 10000), this->map)} {
    mobs.push_back(Mob(Vector<int>(50, 50), this->map));
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
    if (keys[GLFW_KEY_I]) {
        pathfind_to.y -= player_speed;
    }
    if (keys[GLFW_KEY_J]) {
        pathfind_to.x -= player_speed;
    }
    if (keys[GLFW_KEY_K]) {
        pathfind_to.y += player_speed;
    }
    if (keys[GLFW_KEY_L]) {
        pathfind_to.x += player_speed;
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
    
    this->player.move(input);
    
    for (std::vector<Mob>::iterator it = this->mobs.begin(), end = this->mobs.end();
            it != end; it++) {
        (*it).tick(this->player.pos);
    }
    
    // draw the map
    this->map->draw(g);
    
    // g.fill_rectangle(RGBA(0, 255, 0, 255), 100, 300, 200, 400);
    
//    Batch visibility_lines(GL_LINES, RGBA(255, 255, 0, 64), Graphics::shaders[GRAPHICS_COLOUR_SHADER], 10248);
//    for (int i = 0, size = this->map->pathfinder.path_nodes.size(); i < size; i++) {
//        Vector<int> start = this->map->pathfinder.path_nodes[i];
//        for (std::pair<int, int> connection : this->map->pathfinder.path_connections[i]) {
//            Vector<int> end = this->map->pathfinder.path_nodes[connection.first];
//            const GLfloat points[] = {
//                Graphics::game_to_gl_x(start.x),    Graphics::game_to_gl_y(start.y),
//                Graphics::game_to_gl_x(end.x),      Graphics::game_to_gl_y(end.y)
//            };
//            visibility_lines.add(2, points);
//        }
//    }
//    g.draw(visibility_lines);
//    
//    Batch path(GL_LINES, RGBA(255, 255, 255, 255), Graphics::shaders[GRAPHICS_COLOUR_SHADER]);
//    std::list<Vector<int>> nodes = this->map->pathfinder.Dijkstra(this->player.pos, pathfind_to);
//    
//    Vector<int> prev = nodes.front();
//    for(std::list<Vector<int>>::iterator it = nodes.begin()++, end = nodes.end();
//            it != end; it++) {
//        Vector<int> next = *it;
//        const GLfloat points[] = {
//            Graphics::game_to_gl_x(prev.x), Graphics::game_to_gl_y(prev.y),
//            Graphics::game_to_gl_x(next.x), Graphics::game_to_gl_y(next.y)
//        };
//        path.add(2, points);
//        prev = next;
//    }
//    g.draw(path);
    
    // draw mobs
    Batch mob_batch(GL_TRIANGLES, RGBA(0, 255, 0, 255), Graphics::shaders[GRAPHICS_COLOUR_SHADER]);
    for (Mob mob : this->mobs) {
        float xa = Graphics::game_to_gl_x(mob.pos.x - 500), ya = Graphics::game_to_gl_y(mob.pos.y - 500),
              xb = Graphics::game_to_gl_x(mob.pos.x + 500), yb = Graphics::game_to_gl_y(mob.pos.y + 500);

        const GLfloat points[] = {
            xa, ya,
            xb, ya,
            xb, yb,
            xa, ya,
            xa, yb,
            xb, yb
        };
        
        mob_batch.add(6, points);
    }
    g.draw(mob_batch);
    
    this->map->shadow(this->player.pos);
    
    // draw player
    g.fill_rectangle(
            RGBA(128, 128, 128, 255),
            Graphics::game_to_gl_x(this->player.pos.x - 500), Graphics::game_to_gl_y(this->player.pos.y - 500),
            Graphics::game_to_gl_x(this->player.pos.x + 500), Graphics::game_to_gl_y(this->player.pos.y + 500)
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