
#define _GNU_SOURCE
#include <sys/stat.h>
#include <stdio.h>
#include <GLES3/gl3.h>

#include "map.h"
#include "../screens/game.h"

v2i *point_read(char *buf) {
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
    
    v2i *point = malloc(sizeof(v2i));
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
            v2i *point = point_read(buf);
            if(point) {
                list_add_end(points, (list_data_t)(void*)point);
            } else {
                fprintf(stderr, "Invalid point encountered: '%s'\n", buf);
            }
        }
        map->n_points = points->size;
        map->points = malloc(map->n_points * sizeof(v2i));
        printf("%d points\n", map->n_points);
        v2i *point = NULL;
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

ray_collision_t __ray_intersect(v2i p, double m, double c, double cosa, double sina, v2i s1, v2i s2) {
    
    int min_x = min(s1.x, s2.x);
    int max_x = max(s1.x, s2.x);
    int min_y = min(s1.y, s2.y);
    int max_y = max(s1.y, s2.y);
    
    // vertical line segment case
    if(s1.x == s2.x) {
        if(p.x <= s1.x && cosa <= 0 || p.x > s1.x && cosa > 0) no_collision;
        double ey = c + s1.x * m;
        if(ey >= min_y && ey <= max_y) {
            double dy = ey - p.y;
            double dx = s1.x - p.x;
            double dist = sqrt(dy * dy + dx * dx);
            return (ray_collision_t) { s1.x, ey, dist };
        }
        no_collision;
    }
    // calculate the gradient now that we know it's non-zero
    double sm = (double)(s2.y - s1.y) / (s2.x - s1.x);
    double sc = s1.y - sm * s1.x;
    
    // ray starts on the line case
    if(equald(p.y, sm * p.x + sc) && equald(p.y, m * p.x + c) && p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y) {
        // printf("p.y=%d=%d=%d, p.x=%d, min_x=%d, max_x=%d, min_y=%d, max_y=%d\n", p.y, sm * p.x + sc, m * p.x + c, p.x, min_x, max_x, min_y, max_y);
        return (ray_collision_t) { p.x, p.y, 0 };
    }
    // parallel lines case
    if(equald(m, sm)) {
        // check if they're from the same line
        if(!equald(c, sc)) no_collision;
        if(p.x >= min_x && p.x <= max_x) {
            if(p.y >= min_y && p.y <= max_y) return (ray_collision_t) { p.x, p.y, 0 };
            if(p.y <= min_y && sina < 0) return (ray_collision_t) { p.x, min_y, min_y - p.y };
            if(p.y >= max_y && sina > 0) return (ray_collision_t) { p.x, max_y, p.y - max_y };
        }
        if(p.x <= min_x && cosa > 0) {
            if(p.y >= min_y && p.y <= max_y) return (ray_collision_t) { min_x, p.y, min_x - p.x };
            if(p.y <= min_y && sina < 0) {
                double dx = min_x - p.x;
                double dy = min_y - p.y;
                double dist = sqrt(dy * dy + dx * dx);
                return (ray_collision_t) { min_x, min_y, dist };
            }
            if(p.y >= max_y && sina > 0) {
                double dx = min_x - p.x;
                double dy = p.y - max_y;
                double dist = sqrt(dy * dy + dx * dx);
                return (ray_collision_t) { min_x, max_y, dist };
            }
        }
        if(p.x >= max_x && cosa < 0) {
            if(p.y >= min_y && p.y <= max_y) return (ray_collision_t) { max_x, p.y, p.x - max_x };
            if(p.y <= min_y && sina < 0) {
                double dx = p.x - max_x;
                double dy = min_y - p.y;
                double dist = sqrt(dy * dy + dx * dx);
                return (ray_collision_t) { max_x, min_y, dist };
            }
            if(p.y >= max_y && sina > 0) {
                double dx = p.x - max_x;
                double dy = p.y - max_y;
                double dist = sqrt(dy * dy + dx * dx);
                return (ray_collision_t) { max_x, max_y, dist };
            }
        }
        
        no_collision;
    }
    // standard case
    double ex = (sc - c) / (m - sm);
    if(ex < min_x - M_DELTA|| ex > max_x + M_DELTA ||
            ex < p.x && cosa > 0 || ex > p.x && cosa < 0) {
        no_collision;
    }
    double ey = c + ex * m;
    if(ey < min_y - M_DELTA|| ey > max_y + M_DELTA ||
            ey < p.y && sina > 0 || ey > p.y && sina < 0) {
        no_collision;
    }
    double dx = ex - p.x;
    double dy = ey - p.y;
    double dist = sqrt(dx * dx + dy * dy);
    return (ray_collision_t) { ex, ey, dist };
}

ray_collision_t __ray_intersect_v(v2i p, double sina, v2i s1, v2i s2) {
    // parallel lines (vertical line segment) case
    
    int min_x = min(s1.x, s2.x);
    int max_x = max(s1.x, s2.x);
    int min_y = min(s1.y, s2.y);
    int max_y = max(s1.y, s2.y);
    
    if(s2.x == s1.x) {
        if(p.x != s1.x) no_collision;
        if(p.y >= min_y && p.y <= max_y) return (ray_collision_t) { p.x, p.y, 0 };
        if(p.y <= min_y && sina > 0) return (ray_collision_t) { p.x, min_y, min_y - p.y };
        if(p.y >= max_y && sina < 0) return (ray_collision_t) { p.x, max_y, p.y - max_y };
        no_collision;
    }
    // calculate the gradient now that we know it's non-zero
    double sm = (double)(s2.y - s1.y) / (s2.x - s1.x);
    double sc = s1.y - sm * s1.x;
    // ray starts on the line case
    if(equald(p.y, sm * p.x + sc) && p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y) {
        // printf("p.y=%d=%d, p.x=%d, min_x=%d, max_x=%d, min_y=%d, max_y=%d\n", p.y, sm * p.x + sc, min_x, max_x, min_y, max_y);
        return (ray_collision_t) { p.x, p.y, 0 };
    }
    // standard case
    if(p.x >= min_x && p.x <= max_x) {
        double ey = sc + p.x * sm;
        if(p.y >= ey && sina < 0 || p.y <= ey && sina > 0) {
            return (ray_collision_t) { p.x, ey, abs(p.y - ey) };
        }
    }
    no_collision;
}

#undef no_collision

ray_collision_t __raycast(v2i p, double m, double c, double cosa, double sina, map_t *map) {
    int i;
    ray_collision_t collision = (ray_collision_t) { 0, 0, INFINITY };
    ray_collision_t new_collision;
    for(i = 0; i < map->n_segments; i++) {
        new_collision = __ray_intersect(p, m, c, cosa, sina,
                (v2i) { map->points[map->segments[i].a].x, map->points[map->segments[i].a].y },
                (v2i) { map->points[map->segments[i].b].x, map->points[map->segments[i].b].y });
//            if(new_collision.dist != INFINITY) {
//                printf("%f, %f, %f\n", new_collision.x, new_collision.y, new_collision.dist);
//            }
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    }
    return collision;
}

ray_collision_t __raycast_v(v2i p, double sina, map_t *map) {
    int i;
    ray_collision_t collision = (ray_collision_t) { 0, 0, INFINITY };
    ray_collision_t new_collision;
    for(i = 0; i < map->n_segments; i++) {
        new_collision = __ray_intersect_v(p, sina,
                (v2i) { map->points[map->segments[i].a].x, map->points[map->segments[i].a].y },
                (v2i) { map->points[map->segments[i].b].x, map->points[map->segments[i].b].y });
//            if(new_collision.dist != INFINITY) {
//                printf("%f, %f, %f\n", new_collision.x, new_collision.y, new_collision.dist);
//            }
        if(new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    }
    return collision;
}

ray_collision_t map_raycast_a(v2i p, double angle, map_t *map) {
    
    double cosa = cos(angle);
    double sina = -sin(angle);
    
    if(absd(cosa) > M_DELTA) {
        double m = sina / cosa;
        double c = p.y - m * p.x;
        return __raycast(p, m, c, cosa, sina, map);
    } else {
        return __raycast_v(p, sina, map);
    }
}