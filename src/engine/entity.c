
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
    
    v2i pos = v2i_add(e->pos, input);
    
    printf("\ninput: %d, %d\n", input.x, input.y);
    
    int i;
    int rad = e->radius ? e->radius : 1;

    for(i = 0; i < map->n_segments; i++) {
        v2i v = map->points[map->segments[i].a];
        v2i w = map->points[map->segments[i].b];
        bool endpoint;
        double dist = v2i_dist_line_segment(v, w, pos, &endpoint);
        if (dist < rad) {
            if (endpoint) {
                // if we hit the endpoint and the input is not parallel to the vector,
                // don't bother correcting (because we're going right past it)
                double angle_diff = absd(v2i_angle(v2i_sub(v, w)) - v2i_angle(input));
                if (angle_diff > M_PI_4 && angle_diff < 3 * M_PI_4) {
                    continue;
                }
            }
            int side = v2i_side(v, w, e->pos);
            printf("side: %d\n", side);
            v2d normal;
            // find the normal of the line
            if(side == 1) {
                normal = (v2d) { -(double)(w.y - v.y), (double)(w.x - v.x) };
            } else {
                normal = (v2d) { (double)(w.y - v.y), -(double)(w.x - v.x) };
            }
            // normalise
            normal = v2d_scale(1.0 / absd(v2d_mag(normal)), normal);
            printf("normal: %f, %f\n", normal.x, normal.y);
            // correct position
            pos = v2i_add(pos, v2d_to_v2i(v2d_scale(rad - dist + 1, normal)));
        }
    }
    // printf("final: %d, %d\n", input.x, input.y);
    e->pos = pos;
}