
#define _GNU_SOURCE
#include <sys/stat.h>
#include <stdio.h>
#include <GLES3/gl3.h>

#include "map.h"
#include "../screens/game.h"

point_t *point_read(char *buf) {
    if(*buf > '9' || *buf < '0') return NULL;
    char *x = buf;
    // advance to next number
    while(*buf <= '9' && *buf >= '0') ++buf;
    *buf++ = '\0';
    if(*buf > '9' || *buf < '0') return NULL;
    char *y = buf;
    // advance to next number
    while(*buf <= '9' && *buf >= '0') ++buf;
    *buf++ = '\0';
    if(*buf != '\n' && *buf != '\0') return NULL;
    
    point_t *point = malloc(sizeof(point_t));
    point->x = atoi(x);
    point->y = atoi(y);
    
    printf("Point: %d,%d\n", point->x, point->y);
    
    return point;
}

line_segment_t *segment_read(char *buf, map_t *map) {
    if(*buf > '9' || *buf < '0') return NULL;
    char *a = buf;
    // advance to next number
    while(*buf <= '9' && *buf >= '0') ++buf;
    *buf++ = '\0';
    if(*buf > '9' || *buf < '0') return NULL;
    char *b = buf;
    // advance to next number
    while(*buf <= '9' && *buf >= '0') ++buf;
    *buf++ = '\0';
    if(*buf != '\n' && *buf != '\0') return NULL;
    
    line_segment_t *segment = malloc(sizeof(line_segment_t));
    segment->a = atoi(a);
    segment->b = atoi(b);
    
    printf("Line Segment: %d (%d, %d), %d (%d, %d)\n", segment->a, map->points[segment->a].x, map->points[segment->a].y, segment->b, map->points[segment->b].x, map->points[segment->b].y);
    
    return segment;
}

polygon_t *polygon_read(char* buf) {
    list_t *segments = list_init();
    char *id;
    for(;;) {
        if(*buf == '\0' || *buf == '\n') break;
        if(*buf > '9' || *buf < '0') {
            list_free(segments);
            return NULL;
        }
        id = buf;
        while(*buf <= '9' && *buf >= '0') ++buf;
        if(*buf != '\0') *buf++ = '\0';
        list_add(segments, (list_data_t)(void*)id);
    }
    polygon_t *polygon = malloc(sizeof(polygon_t));
    polygon->n_segments = segments->size;
    polygon->segment_ids = malloc(polygon->n_segments * sizeof(int));
    int i = 0;
    while(id = list_remove(segments).data) {
        polygon->segment_ids[i++] = atoi(id);
    }
    list_free(segments);
    
    printf("Polygon with %d line segments\n", polygon->n_segments);
    
    return polygon;
}

map_t* map_open(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fputs("Failed to read map\n", stderr);
        return NULL;
    }
    
    char* buf = NULL;
    size_t n = 0;
    __ssize_t len;
    
    map_t *map = malloc(sizeof(map_t));
    
    // read points
    {
        list_t *points = list_init();
        while ((len = getline(&buf, &n, file)) != -1) {
            if(*buf == '\0' || *buf == '\n') break;
            point_t *point = point_read(buf);
            if(point) {
                list_add_end(points, (list_data_t)(void*)point);
            } else {
                fprintf(stderr, "Invalid point encountered: '%s'\n", buf);
            }
        }
        map->n_points = points->size;
        map->points = malloc(map->n_points * sizeof(point_t));
        printf("%d points\n", map->n_points);
        point_t *point = NULL;
        int i = 0;
        while(point = list_remove(points).data) map->points[i++] = *point;
        list_free_all(points);
    }
    
    // read line segments
    {
        list_t *line_segments = list_init();
        while ((len = getline(&buf, &n, file)) != -1) {
            if(*buf == '\0' || *buf == '\n') break;
            line_segment_t *segment = segment_read(buf, map);
            if(segment) {
                list_add_end(line_segments, (list_data_t)(void*)segment);
            } else {
                fprintf(stderr, "Invalid line segment encountered: '%s'\n", buf);
            }
        }
        map->n_segments = line_segments->size;
        map->segments = malloc(map->n_segments * sizeof(line_segment_t));
        printf("%d lines\n", map->n_segments);
        line_segment_t *segment = NULL;
        int i = 0;
        while(segment = list_remove(line_segments).data) map->segments[i++] = *segment;
        list_free_all(line_segments);
    }
    
    // read polygons
    {
        map->polygons = list_init();
        while ((len = getline(&buf, &n, file)) != -1) {
            if(*buf == '\0' || *buf == '\n') break;
            polygon_t *polygon = polygon_read(buf);
            if(polygon) {
                list_add(map->polygons, (list_data_t)(void*)polygon);
            } else {
                fprintf(stderr, "Invalid polygon encountered: '%s'\n", buf);
            }
        }
    }
    
    free(buf);
    fclose(file);
    
    printf("loaded %s\n", filename);
    return map;
}

#define no_collision return (ray_collision_t) { 0, 0, INFINITY };

ray_collision_t ray_intersection(double x, double y, double m, double c, double cosa, double sina, double x1, double y1, double x2, double y2) {
    // vertical line segment case
    if(x2 == x1) {
        if(x < x1 && cosa < 0 || x > x1 && cosa > 0) no_collision;
        double ey = c + x1 * m;
        if(ey >= min(y1, y2) && ey <= max(y1, y2)) {
            double dy = ey - y;
            double dx = x1 - x;
            double dist = sqrt(dy * dy + dx * dx);
            return (ray_collision_t) { x1, ey, dist };
        }
        no_collision;
    }
    // calculate the gradient now that we know it's non-zero
    double sm = (y2 - y1) / (x2 - x1);
    double sc = y1 - sm * x1;
    // parallel lines case
    if(equald(m, sm)) {
        // check if they're from the same line
        if(!equald(c, sc)) no_collision;
        double min_x = min(x1, x2);
        double max_x = max(x1, x2);
        double min_y = min(y1, y2);
        double max_y = max(y1, y2);
        if(x >= min_x && x <= max_x) {
            if(y >= min_y && y <= max_y) return (ray_collision_t) { x, y, 0 };
            if(y <= min_y && sina < 0) return (ray_collision_t) { x, min_y, min_y - y };
            if(y >= max_y && sina > 0) return (ray_collision_t) { x, max_y, y - max_y };
        }
        if(x <= min_x && cosa > 0) {
            if(y >= min_y && y <= max_y) return (ray_collision_t) { min_x, y, min_x - x };
            if(y <= min_y && sina < 0) {
                double dx = min_x - x;
                double dy = min_y - y;
                double dist = sqrt(dy * dy + dx * dx);
                return (ray_collision_t) { min_x, min_y, dist };
            }
            if(y >= max_y && sina > 0) {
                double dx = min_x - x;
                double dy = y - max_y;
                double dist = sqrt(dy * dy + dx * dx);
                return (ray_collision_t) { min_x, max_y, dist };
            }
        }
        if(x >= max_x && cosa < 0) {
            if(y >= min_y && y <= max_y) return (ray_collision_t) { max_x, y, x - max_x };
            if(y <= min_y && sina < 0) {
                double dx = x - max_x;
                double dy = min_y - y;
                double dist = sqrt(dy * dy + dx * dx);
                return (ray_collision_t) { max_x, min_y, dist };
            }
            if(y >= max_y && sina > 0) {
                double dx = x - max_x;
                double dy = y - max_y;
                double dist = sqrt(dy * dy + dx * dx);
                return (ray_collision_t) { max_x, max_y, dist };
            }
        }
        
        no_collision;
    }
    // standard case
    double ex = (sc - c) / (m - sm);
    if(ex < min(x1, x2) - 0.001 || ex > max(x1, x2) + 0.001 ||
            ex < x && cosa > 0 || ex > x && cosa < 0) {
        no_collision;
    }
    double ey = c + ex * m;
    if(ey < min(y1, y2) - 0.001 || ey > max(y1, y2) + 0.001 ||
            ey < y && sina > 0 || ey > y && sina < 0) {
        no_collision;
    }
    double dx = ex - x;
    double dy = ey - y;
    double dist = sqrt(dx * dx + dy * dy);
    return (ray_collision_t) { ex, ey, dist };
}

ray_collision_t vertical_ray_intersection(double x, double y, double sina,
        double x1, double y1, double x2, double y2) {
    // parallel lines (vertical line segment) case
    if(x2 == x1) {
        if(x != x1) no_collision;
        double min_y = min(y1, y2);
        double max_y = max(y1, y2);
        if(y >= min_y && y <= max_y) return (ray_collision_t) { x, y, 0 };
        if(y <= min_y && sina > 0) return (ray_collision_t) { x, min_y, min_y - y };
        if(y >= max_y && sina < 0) return (ray_collision_t) { x, max_y, y - max_y };
        no_collision;
    }
    // calculate the gradient now that we know it's non-zero
    double sm = (y2 - y1) / (x2 - x1);
    double sc = y1 - sm * x1;
    // standard case
    if(x >= min(x1, x2) && x <= max(x1, x2)) {
        double ey = sc + x * sm;
        if(y >= ey && sina < 0 || y <= ey && sina > 0) {
            return (ray_collision_t) { x, ey, abs(y - ey) };
        }
    }
    no_collision;
}

#undef no_collision

ray_collision_t map_raycast_a(double x, double y, double angle, map_t *map) {
    
    int i;
    ray_collision_t closest_collision = (ray_collision_t) { 0, 0, INFINITY };
    ray_collision_t new_collision;
    double cosa = cos(angle);
    double sina = -sin(angle);
    
    // printf("angle=%f, cos(angle)=%f, sin(angle)=%f\n", angle, cosa, sina);
    if(absd(cosa) > delta) {
        double m = sina / cosa;
        double c = y - m * x;
        // check edges of the map
        // top
        new_collision = ray_intersection(x, y, m, c, cosa, sina, 0, 0, RENDER.width, 0);
        if(new_collision.dist < closest_collision.dist) {
            closest_collision = new_collision;
        }
        // right
        new_collision = ray_intersection(x, y, m, c, cosa, sina, RENDER.width, 0, RENDER.width, RENDER.height);
        if(new_collision.dist < closest_collision.dist) {
            closest_collision = new_collision;
        }
        // bottom
        new_collision = ray_intersection(x, y, m, c, cosa, sina, RENDER.width, RENDER.height, 0, RENDER.height);
        if(new_collision.dist < closest_collision.dist) {
            closest_collision = new_collision;
        }
        // left
        new_collision = ray_intersection(x, y, m, c, cosa, sina, 0, RENDER.height, 0, 0);
        if(new_collision.dist < closest_collision.dist) {
            closest_collision = new_collision;
        }
        // check the line segments
        for(i = 0; i < map->n_segments; i++) {
            new_collision = ray_intersection(x, y, m, c, cosa, sina,
                    map->points[map->segments[i].a].x, map->points[map->segments[i].a].y,
                    map->points[map->segments[i].b].x, map->points[map->segments[i].b].y);
//            if(new_collision.dist != INFINITY) {
//                printf("%f, %f, %f\n", new_collision.x, new_collision.y, new_collision.dist);
//            }
            if(new_collision.dist < closest_collision.dist) {
                closest_collision = new_collision;
            }
        }
    } else {
        // check edges of the map
        // top
        new_collision = vertical_ray_intersection(x, y, sina, 0, 0, RENDER.width, 0);
        if(new_collision.dist < closest_collision.dist) {
            closest_collision = new_collision;
        }
        // right
        new_collision = vertical_ray_intersection(x, y, sina, RENDER.width, 0, RENDER.width, RENDER.height);
        if(new_collision.dist < closest_collision.dist) {
            closest_collision = new_collision;
        }
        // bottom
        new_collision = vertical_ray_intersection(x, y, sina, RENDER.width, RENDER.height, 0, RENDER.height);
        if(new_collision.dist < closest_collision.dist) {
            closest_collision = new_collision;
        }
        // left
        new_collision = vertical_ray_intersection(x, y, sina, 0, RENDER.height, 0, 0);
        if(new_collision.dist < closest_collision.dist) {
            closest_collision = new_collision;
        }
        // check the line segments
        for(i = 0; i < map->n_segments; i++) {
            new_collision = vertical_ray_intersection(x, y, sina,
                    map->points[map->segments[i].a].x, map->points[map->segments[i].a].y,
                    map->points[map->segments[i].b].x, map->points[map->segments[i].b].y);
//            if(new_collision.dist != INFINITY) {
//                printf("%f, %f, %f\n", new_collision.x, new_collision.y, new_collision.dist);
//            }
            if(new_collision.dist < closest_collision.dist) {
                closest_collision = new_collision;
            }
        }
    }
    
    // printf("%f, %f, %f\n", closest_collision.x, closest_collision.y, closest_collision.dist);
    return closest_collision;
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
    glColor4ub(0, 255, 255, 100);
    glBegin(GL_POLYGON);
    glVertex2d(game_to_gl_x(game_data.player.x), game_to_gl_y(game_data.player.y));
    double angle = list_remove(angles).dvalue;
    ray_collision_t first = map_raycast_a(x, y, angle, game_data.map);
    glVertex2d(game_to_gl_x(first.x), game_to_gl_y(first.y));
    while(angles->size > 0) {
        double angle = list_remove(angles).dvalue;
        // printf("%f\n", angle);
        ray_collision_t collision = map_raycast_a(x, y, angle, game_data.map);
        glVertex2d(game_to_gl_x(collision.x), game_to_gl_y(collision.y));
    }
    list_free(angles);
    glVertex2d(game_to_gl_x(first.x), game_to_gl_y(first.y));
    glVertex2d(game_to_gl_x(game_data.player.x), game_to_gl_y(game_data.player.y));
    glEnd();
    // printf("\n\n\n\n\n");
}