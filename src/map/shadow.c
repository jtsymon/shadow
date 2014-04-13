
#include "shadow.h"

ray_collision_t shadow_raycast(double x, double y, double angle) {
    
    double cosa = cos(angle);
    double sina = -sin(angle);
    ray_collision_t collision = (ray_collision_t) { 0, 0, INFINITY };
    ray_collision_t new_collision;
    
    if(absd(cosa) > delta) {
        double m = sina / cosa;
        double c = y - m * x;
        // check edges of the map
        // top
        new_collision = __ray_intersect(x, y, m, c, cosa, sina, 0, 0, RENDER.width, 0);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // right
        new_collision = __ray_intersect(x, y, m, c, cosa, sina, RENDER.width, 0, RENDER.width, RENDER.height);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // bottom
        new_collision = __ray_intersect(x, y, m, c, cosa, sina, RENDER.width, RENDER.height, 0, RENDER.height);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // left
        new_collision = __ray_intersect(x, y, m, c, cosa, sina, 0, RENDER.height, 0, 0);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        new_collision = __raycast(x, y, m, c, cosa, sina, game_data.map);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    } else {
        // check edges of the map
        // top
        new_collision = __ray_intersect_v(x, y, sina, 0, 0, RENDER.width, 0);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // right
        new_collision = __ray_intersect_v(x, y, sina, RENDER.width, 0, RENDER.width, RENDER.height);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // bottom
        new_collision = __ray_intersect_v(x, y, sina, RENDER.width, RENDER.height, 0, RENDER.height);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // left
        new_collision = __ray_intersect_v(x, y, sina, 0, RENDER.height, 0, 0);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        new_collision = __raycast_v(x, y, sina, game_data.map);
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    }
    return collision;
}

int sorter(list_data_t data1, list_data_t data2) {
    return data1.dvalue >= data2.dvalue ? 1 : -1;
}

void map_shadow(double x, double y) {
    
    int i;
    list_t *angles = list_init();
    for(i = 0; i < game_data.map->n_points; i++) {
        double angle = atan2(y - game_data.map->points[i].y,
                game_data.map->points[i].x - x);
        list_add(angles, (list_data_t)angle_sanify(angle - delta * 1000));
        list_add(angles, (list_data_t)angle_sanify(angle));
        list_add(angles, (list_data_t)angle_sanify(angle + delta * 1000));
    }
    // check edges of the screen
    list_add(angles, (list_data_t)angle_sanify(atan2(y, -x)));
    list_add(angles, (list_data_t)angle_sanify(atan2(y, RENDER.width - x)));
    list_add(angles, (list_data_t)angle_sanify(atan2(y - RENDER.height, RENDER.width - x)));
    list_add(angles, (list_data_t)angle_sanify(atan2(y - RENDER.height, -x)));
    list_quicksort_f(angles, sorter);
    glDisable(GL_BLEND);
    glColor4ub(100, 100, 100, 255);
    glBegin(GL_POLYGON);
    glVertex2d(game_to_gl_x(game_data.player.x), game_to_gl_y(game_data.player.y));
    double angle = list_remove(angles).dvalue;
    ray_collision_t first = shadow_raycast(x, y, angle);
    glVertex2d(game_to_gl_x(first.x), game_to_gl_y(first.y));
    while(angles->size > 0) {
        double angle = list_remove(angles).dvalue;
        // printf("%f\n", angle);
        ray_collision_t collision = shadow_raycast(x, y, angle);
        double px = collision.x;
        double py = collision.y;
//        if(angle > M_PI_2 && angle < M_PI_2 * 3) px += 2;
//        else px -= 2;
//        if(angle > M_PI) py -= 2;
//        else py += 2;
        glVertex2d(game_to_gl_x(px), game_to_gl_y(py));
    }
    list_free(angles);
    glVertex2d(game_to_gl_x(first.x), game_to_gl_y(first.y));
    glVertex2d(game_to_gl_x(game_data.player.x), game_to_gl_y(game_data.player.y));
    glEnd();
    glEnable(GL_BLEND);
    // printf("\n\n\n\n\n");
}