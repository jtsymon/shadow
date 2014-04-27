
#include "entity.h"

entity_t entity_init(int x, int y) {
    entity_t e;
    e.pos.x = x;
    e.pos.y = y;
    e.radius = 10;
    return e;
}

void entity_move(entity_t *e, int dx, int dy, map_t *map) {
    
    if(dx == 0 && dy == 0) return;
    
    vec2 input = { dx, dy };
    vec2 pos = vec_sub(e->pos, input);
    
    printf("\ninput: %f, %f\n", input.x, input.y);
    
    int i;
    int rad = e->radius ? e->radius : 1;

    for(i = 0; i < map->n_segments; i++) {
        vec2 v = map->points[map->segments[i].a];
        vec2 w = map->points[map->segments[i].b];
        if(dist_line_segment(v, w, pos) < rad) {
            vec2 normal;
            int side = vec_side(v, w, pos);
            printf("side: %d\n", side);
            if(side == 1) {
                normal = (vec2) { -(w.y - v.y), -(w.x - v.x) };
            } else {
                normal = (vec2) { w.y - v.y, -(w.x - v.x) };
            }
            normal = vec_scale(1.0 / absd(vec_mag(normal)), normal);
            printf("normal: %f, %f\n", normal.x, normal.y);
            if(side == 1) {
                normal.y *= -1;
                printf("normal: %f, %f\n", normal.x, normal.y);
            }
            vec2 undesired = vec_scale(vec_dot(input, normal), normal);
            printf("undesired: %f, %f\n", undesired.x, undesired.y);
            if(input.y > M_DELTA && normal.y > M_DELTA || input.y < -M_DELTA && normal.y < -M_DELTA) undesired.y = 0;
            if(input.x > M_DELTA && normal.x > M_DELTA || input.x < -M_DELTA && normal.x < -M_DELTA) undesired.x = 0;
            printf("undesired: %f, %f\n", undesired.x, undesired.y);
            input = vec_sub(input, undesired);
            printf("input: %f, %f\n", input.x, input.y);
            pos = vec_sub(e->pos, input);
        }
    }
    printf("final: %f, %f\n", input.x, input.y);
    e->pos = vec_add(e->pos, input);
}