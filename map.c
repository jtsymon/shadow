
#include "map.h"

map_t* map_init(int width, int height, unsigned char data[]) {
    map_t *map = malloc(sizeof(int) * 2 + sizeof(unsigned char*));
    map->width = width;
    map->height = height;
    map->data = data;
    return map;
}