
#include "entity.h"

entity_t entity_init(int x, int y) {
    entity_t e;
    e.pos.x = x;
    e.pos.y = y;
    e.radius = 10;
    return e;
}

void entity_move(entity_t *e, v2i input, map_t *map) {
    
    if(input.x == 0 && input.y == 0) return;
    
    v2i pos = v2i_sub(e->pos, input);
    
    printf("\ninput: %d, %d\n", input.x, input.y);
    
    int i;
    int rad = e->radius ? e->radius : 1;

    for(i = 0; i < map->n_segments; i++) {
        v2i v = map->points[map->segments[i].a];
        v2i w = map->points[map->segments[i].b];
        if(v2i_dist_line_segment(v, w, pos) < rad) {
            int side = v2i_side(v, w, pos);
            printf("side: %d\n", side);
            v2d normal;
            if(side == 1) {
                normal = (v2d) { -(double)(w.y - v.y), -(double)(w.x - v.x) };
            } else {
                normal = (v2d) { (double)(w.y - v.y), -(double)(w.x - v.x) };
            }
            normal = v2d_scale(1.0 / absd(v2d_mag(normal)), normal);
            printf("normal: %f, %f\n", normal.x, normal.y);
            if(side == 1) {
                normal.y *= -1;
                printf("normal: %f, %f\n", normal.x, normal.y);
            }
            v2d undesired = v2d_scale(v2d_dot(v2i_to_v2d(input), normal), normal);
            printf("undesired: %f, %f\n", undesired.x, undesired.y);
            if(input.y > 0 && normal.y > M_DELTA || input.y < 0 && normal.y < -M_DELTA) undesired.y = 0;
            if(input.x > 0 && normal.x > M_DELTA || input.x < 0 && normal.x < -M_DELTA) undesired.x = 0;
            printf("undesired: %f, %f\n", undesired.x, undesired.y);
            input = v2i_sub(input, v2d_to_v2i(undesired));
            printf("input: %d, %d\n", input.x, input.y);
            pos = v2i_sub(e->pos, input);
        }
    }
    printf("final: %d, %d\n", input.x, input.y);
    e->pos = v2i_add(e->pos, input);
}