
//#include "visibility.h"
//
//void print_line_segment(line_segment_t* ls) {
//    printf("%d,%d -> %d,%d\n", ls->x1, ls->y1, ls->x2, ls->y2);
//}
//
//line_segment_t* new_line_segment(int x1, int y1, int x2, int y2) {
//    line_segment_t* ls = malloc(sizeof (line_segment_t));
//    ls->x1 = x1;
//    ls->y1 = y1;
//    ls->x2 = x2;
//    ls->y2 = y2;
//    return ls;
//}
//
//void map_line_segments(map_t* map) {
//    // outer bounding box around whole map
//    list_add_end(map->segments, new_line_segment(0, 0, map->width, 0));
//    list_add_end(map->segments, new_line_segment(map->width, 0, map->width, map->height));
//    list_add_end(map->segments, new_line_segment(map->width, map->height, 0, map->height));
//    list_add_end(map->segments, new_line_segment(0, map->height, 0, 0));
//    // horizontal faces (top/bottom)
//    for (int y = 0; y < map->height; y++) {
//        // top
//        if (y > 0) {
//            for (int x = 0; x < map->width; x++) {
//                if (map->data[x + y * map->width] != 0 && map->data[x + (y - 1) * map->width] == 0) {
//                    int start = x;
//                    while (map->data[++x + y * map->width] != 0 && map->data[x + (y - 1) * map->width] == 0);
//                    list_add_end(map->segments, new_line_segment(start, y, x, y));
//                }
//            }
//        }
//        // bottom
//        if (y < map->height - 1) {
//            for (int x = 0; x < map->width; x++) {
//                if (map->data[x + y * map->width] != 0 && map->data[x + (y + 1) * map->width] == 0) {
//                    int start = x;
//                    while (map->data[++x + y * map->width] != 0 && map->data[x + (y + 1) * map->width] == 0);
//                    list_add_end(map->segments, new_line_segment(start, y, x, y));
//                }
//            }
//        }
//    }
//    // vertical faces (left/right)
//    for (int x = 0; x < map->width; x++) {
//        // left
//        if (x > 0) {
//            for (int y = 0; y < map->height; y++) {
//                if (map->data[x + y * map->width] != 0 && map->data[x - 1 + y * map->width] == 0) {
//                    int start = y;
//                    while (map->data[x + ++y * map->width] != 0 && map->data[x - 1 + y * map->width] == 0);
//                    list_add_end(map->segments, new_line_segment(x, start, x, y));
//                }
//            }
//        }
//        // right
//        if (x < map->width - 1) {
//            for (int y = 0; y < map->height; y++) {
//                if (map->data[x + y * map->width] != 0 && map->data[x + 1 + y * map->width] == 0) {
//                    int start = y;
//                    while (map->data[x + ++y * map->width] != 0 && map->data[x + 1 + y * map->width] == 0);
//                    list_add_end(map->segments, new_line_segment(x, start, x, y));
//                }
//            }
//        }
//    }
//
//
//    // DEBUG
//    list_foreach(map->segments, &print_line_segment);
//}