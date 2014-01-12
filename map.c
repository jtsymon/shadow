
#include <sys/stat.h>

#include "screens/game.h"
#include "map.h"

map_t* map_init(int width, int height, unsigned char data[]) {
    map_t* map = malloc(sizeof (map_t));
    map->width = width;
    map->height = height;
    map->data = malloc(width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            map->data[x + width * y] = data[x + width * y];
        }
    }
    return map;
}

map_t* map_open(char* filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        fprintf(stderr, "Cannot determine size of %s: %s\n", filename, strerror(errno));
        return NULL;
    }
    int file_size = st.st_size;

    FILE* map_file = fopen(filename, "rb");
    if (map_file == NULL) {
        fprintf(stderr, "Cannot open %s for binary reading: %s\n", filename, strerror(errno));
    }

    char* data = malloc(file_size);
    fread(data, 1, file_size, map_file);
    fclose(map_file);

    int p = 0;
    int width = data[p++] << 8 | data[p++];
    int height = data[p++] << 8 | data[p++];

    map_t* map = malloc(sizeof (map_t));
    map->width = width;
    map->height = height;
    int map_size = width * height;
    map->data = malloc(map_size);
    for (int i = 0; i < map_size; i++) {
        map->data[i] = data[p++];
    }
    free(data);
    return map;
}

/**
 * Cast a ray from ($x, $y) on the map, in direction $angle
 * @param angle
 * @param x
 * @param y
 * @return details about the collision
 */
map_tile_collision map_raycast(double angle, double x, double y) {

    int ix = (int) x, iy = (int) y;
    double dx = cos(angle);
    double dy = sin(angle);
    double middle = 0;

    // printf("%f - %f,%f (%d,%d)\n", angle, x, y, ix, iy);

    // Work out which direction to go
    if (dx >= 0 && dy >= 0) {
        // Top right quadrant
        // printf("TopRight\n");

        while (ix < game_data.map->width && iy >= 0 && game_data.map->data[ix + game_data.map->width * iy] == 0) {

            middle = vector_to_angle((double) ix + 1 - x, (double) iy - y);

            // printf("%f (%f) - %f,%f (%d,%d)\n", middle, angle, x, y, ix, iy);

            if (angle < middle || angle == middle && angle == 0) {
                // exiting through right of tile
                ix++;
            } else {
                // exiting through top of tile
                iy--;
            }
        }

        // We hit square ix, iy, we can use last calculated middle to work out
        // which side was hit.
        if (angle < middle || angle == 0) {
            // Hit left side

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_left,
                        ix * game_data.tile_size,
                        (y - dy * (ix - x) / dx) * game_data.tile_size
            };
        } else if (angle == middle) {
            // Hit the corner of a tile,
            // so we check the angle to that tile to figure out what side we hit
            middle = vector_to_angle((double) ix + 1 - x, (double) iy - y);
            if (angle <= middle) {
                if (angle == middle) {
                    // this shouldn't happen
                    printf("Error calculating angle: %f - %d,%d\n", angle, ix, iy);
                }
                // Hit bottom
                iy++;

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_left,
                            ix * game_data.tile_size,
                            iy * game_data.tile_size
                };
            } else {
                // Hit left side

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_left,
                            ix * game_data.tile_size,
                            iy * game_data.tile_size
                };
            }
        } else {
            // Hit bottom
            iy++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_bottom,
                        (x - dx * (iy - y) / dy) * game_data.tile_size,
                        iy * game_data.tile_size
            };
        }

    } else if (dx < 0 && dy >= 0) {
        // Top left quadrant
        // printf("TopLeft\n");

        if (ix == x) {
            ix--;
        }

        while (ix >= 0 && iy >= 0 && game_data.map->data[ix + game_data.map->width * iy] == 0) {

            middle = vector_to_angle((double) ix - x, (double) iy - y);

            // printf("%f (%f) - %f,%f (%d,%d)\n", middle, angle, x, y, ix, iy);

            if (angle < middle || angle == middle && angle == M_PI) {
                // exiting through top of tile
                iy--;
            } else {
                // exiting through left of tile
                ix--;
            }
        }

        // We hit square ix, iy, we can use last calculated middle to work out
        // which side was hit.
        if (angle == middle) {
            // Hit the corner of a tile,
            // so we check the angle to that tile to figure out what side we hit
            middle = vector_to_angle((double) ix - x, (double) iy - y);
            if (angle >= middle) {
                if (angle == middle) {
                    // this shouldn't happen
                    printf("Error calculating angle: %f - %d,%d\n", angle, ix, iy);
                }
                // Hit bottom side
                iy++;

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_bottom,
                            ix * game_data.tile_size,
                            iy * game_data.tile_size
                };
            } else {
                // Hit right side
                ix++;

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_right,
                            ix * game_data.tile_size,
                            iy * game_data.tile_size
                };
            }
        } else if (angle > middle) {
            // Hit right side
            ix++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_right,
                        ix * game_data.tile_size,
                        (y - dy * (ix - x) / dx) * game_data.tile_size
            };
        } else {
            // Hit bottom
            iy++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_bottom,
                        (x - dx * (iy - y) / dy) * game_data.tile_size,
                        iy * game_data.tile_size
            };
        }

    } else if (dx < 0 && dy < 0) {
        // Bottom left quadrant
        // printf("BottomLeft\n");

        if (ix == x) {
            ix--;
        }

        while (ix >= 0 && iy < game_data.map->height && game_data.map->data[ix + game_data.map->width * iy] == 0) {

            middle = vector_to_angle((double) ix - x, (double) iy + 1 - y);

            // printf("%f (%f) - %f,%f (%d,%d)\n", middle, angle, x, y, ix, iy);

            if (angle < middle || angle == middle && angle == M_PI_2 * 3) {
                // exiting through left of tile
                ix--;
            } else {
                // exiting through bottom of tile
                iy++;
            }
        }

        // We hit square ix, iy, we can use last calculated middle to work out
        // which side was hit.
        if (angle == middle) {
            // Hit the corner of a tile,
            // so we check the angle to that tile to figure out what side we hit
            middle = vector_to_angle((double) ix - x, (double) iy + 1 - y);
            if (angle >= middle) {
                if (angle == middle) {
                    // this shouldn't happen
                    printf("Error calculating angle: %f - %d,%d\n", angle, ix, iy);
                }
                // Hit right side
                ix++;

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_right,
                            ix * game_data.tile_size,
                            iy * game_data.tile_size
                };
            } else {
                // Hit top side

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_top,
                            ix * game_data.tile_size,
                            iy * game_data.tile_size
                };
            }
        } else if (angle > middle) {
            // Hit top

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_top,
                        (x - dx * (iy - y) / dy) * game_data.tile_size,
                        iy * game_data.tile_size
            };
        } else {
            // Hit right side
            ix++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_right,
                        ix * game_data.tile_size,
                        (y - dy * (ix - x) / dx) * game_data.tile_size
            };
        }

    } else if (dx >= 0 && dy < 0) {
        // Bottom right quadrant
        // printf("BottomRight\n");

        while (ix < game_data.map->width && iy < game_data.map->height && game_data.map->data[ix + game_data.map->width * iy] == 0) {

            middle = vector_to_angle((double) ix + 1 - x, (double) iy + 1 - y);

            // printf("%f (%f) - %f,%f (%d,%d)\n", middle, angle, x, y, ix, iy);

            if (angle < middle || angle == middle && angle == M_PI * 2) {
                // exiting through bottom of tile
                iy++;

            } else {
                // exiting through right of tile
                ix++;
            }
        }

        // We hit square ix, iy, we can use last calculated middle to work out
        // which side was hit.
        if (angle == middle) {
            // Hit the corner of a tile,
            // so we check the angle to that tile to figure out what side we hit
            middle = vector_to_angle((double) ix - x, (double) iy + 1 - y);
            if (angle >= middle) {
                if (angle == middle) {
                    // this shouldn't happen
                    printf("Error calculating angle: %f - %d,%d\n", angle, ix, iy);
                }
                // Hit top side
                // printf("top\n");

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_top,
                            ix * game_data.tile_size,
                            iy * game_data.tile_size
                };
            } else {
                // Hit left side

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_left,
                            ix * game_data.tile_size,
                            iy * game_data.tile_size
                };
            }
        } else if (angle > middle) {
            // Hit left side

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_top,
                        ix * game_data.tile_size,
                        (y - dy * (ix - x) / dx) * game_data.tile_size
            };
        } else {
            // Hit right side
            ix++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_right,
                        (x - dx * (iy - y) / dy) * game_data.tile_size,
                        iy * game_data.tile_size
            };
        }

    } // End of finding square collision.

}

bool debug_shadow = true;

/**
 * Figure out which areas of the map are visible to the player
 * @param x centre x
 * @param y centre y
 */
void map_shadow(int x, int y) {
    // create an alpha mask pixmap
    // Pixmap data = new Pixmap(BERGS.width, BERGS.height, Pixmap.Format.Alpha);
    // byte[] pixels = new byte[BERGS.width * BERGS.height];
    // Arrays.fill(pixels, (byte) 255);

    int p2x = 0, p2y = 0, p1x = 0, p1y = 0;
    map_tile_collision tile;
    double sx = (double) x / game_data.tile_size;
    double sy = (double) y / game_data.tile_size;
    double min_angle = 0, tmp_angle;
    int iter = 0;

    // unmask the regions that should be visible
    while (min_angle < 2 * M_PI) {
        iter++;
        // printf("%d\n", min_angle);

        tile = map_raycast(min_angle, sx, sy);
        // get the whole wall efficiently
        int ix, iy;
        
        set_color(255, 0, 255);
        
        // printf("%d:%d %d:%d", tile.x, ix, tile.y, iy);
        switch (tile.side) {
            case map_tile_left: // right
                ix = (int) tile.x;

                // go downwards to get p2
                iy = (int) tile.y + 1;
                while (true) {
                    if (debug_shadow) {
                        fill_rectangle(ix * game_data.tile_size - 1, iy * game_data.tile_size - 1,
                                ix * game_data.tile_size + 1, iy * game_data.tile_size + 1);
                    }

                    if (game_data.map->data[ix + game_data.map->width * --iy] == 0) {
                        iy++;
                        break;
                    }
                    if (game_data.map->data[ix - 1 + game_data.map->width * iy] != 0) {
                        iy++;
                        break;
                    }
                    tmp_angle = vector_to_angle(ix - sx, iy - sy);
                    if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                        min_angle = tmp_angle;
                    }
                    if (map_raycast(tmp_angle, sx, sy).x != tile.x) {
                        break;
                    }
                }
                p2x = (int) (ix * game_data.tile_size);
                p2y = (int) (iy * game_data.tile_size);

                // go upwards to get p1
                iy = (int) tile.y;
                while (true) {
                    if (debug_shadow) {
                        fill_rectangle(ix * game_data.tile_size - 1, iy * game_data.tile_size - 1,
                                ix * game_data.tile_size + 1, iy * game_data.tile_size + 1);
                    }

                    if (game_data.map->data[ix + game_data.map->width * ++iy] == 0) {
                        break;
                    }
                    if (game_data.map->data[ix - 1 + game_data.map->width * iy] != 0) {
                        break;
                    }
                    tmp_angle = vector_to_angle(ix - sx, iy - sy);
                    if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                        min_angle = tmp_angle;
                    }
                    if (map_raycast(tmp_angle, sx, sy).x != tile.x) {
                        break;
                    }
                }
                p1x = (int) (ix * game_data.tile_size);
                p1y = (int) (iy * game_data.tile_size);
                break;

            case map_tile_bottom: // up
                iy = (int) tile.y;

                // go left to get p2
                ix = (int) tile.x + 1;
                while (true) {
                    if (debug_shadow) {
                        fill_rectangle(ix * game_data.tile_size - 1, iy * game_data.tile_size - 1,
                                ix * game_data.tile_size + 1, iy * game_data.tile_size + 1);
                    }

                    if (game_data.map->data[--ix + game_data.map->width * iy] == 0) {
                        ix++;
                        break;
                    }
                    if (game_data.map->data[ix + game_data.map->width * (iy - 1)] != 0) {
                        ix++;
                        break;
                    }
                    tmp_angle = vector_to_angle(ix - sx, iy - sy);
                    if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                        min_angle = tmp_angle;
                    }
                    if (map_raycast(tmp_angle, sx, sy).y != tile.y) {
                        break;
                    }
                }
                p2x = (int) (ix * game_data.tile_size);
                p2y = tile.pY;

                // go right to get p1
                ix = (int) tile.x;
                while (true) {
                    if (debug_shadow) {
                        fill_rectangle(ix * game_data.tile_size - 1, iy * game_data.tile_size - 1,
                                ix * game_data.tile_size + 1, iy * game_data.tile_size + 1);
                    }

                    if (game_data.map->data[++ix + game_data.map->width * iy] == 0) {
                        break;
                    }
                    if (game_data.map->data[ix + game_data.map->width * (iy - 1)] != 0) {
                        break;
                    }
                    tmp_angle = vector_to_angle(ix - sx, iy - sy);
                    if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                        min_angle = tmp_angle;
                    }
                    if (map_raycast(tmp_angle, sx, sy).y != tile.y) {
                        break;
                    }
                }
                p1x = (int) (ix * game_data.tile_size);
                p1y = tile.pY;
                break;

            case map_tile_right: // left
                ix = (int) tile.x - 1;

                // go downwards to get p2
                iy = (int) tile.y + 1;
                while (true) {
                    if (debug_shadow) {
                        fill_rectangle(ix * game_data.tile_size - 1, iy * game_data.tile_size - 1,
                                ix * game_data.tile_size + 1, iy * game_data.tile_size + 1);
                    }

                    if (game_data.map->data[ix + game_data.map->width * --iy] == 0) {
                        iy++;
                        break;
                    }
                    if (game_data.map->data[ix + 1 + game_data.map->width * iy] != 0) {
                        iy++;
                        break;
                    }
                    tmp_angle = vector_to_angle(ix - sx, iy - sy);
                    if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                        min_angle = tmp_angle;
                    }
                    if (map_raycast(tmp_angle, sx, sy).x != tile.x) {
                        break;
                    }
                }
                p2x = tile.pX;
                p2y = (int) (iy * game_data.tile_size);

                // go upwards to get p1
                iy = (int) tile.y;
                while (true) {
                    if (debug_shadow) {
                        fill_rectangle(ix * game_data.tile_size - 1, iy * game_data.tile_size - 1,
                                ix * game_data.tile_size + 1, iy * game_data.tile_size + 1);
                    }

                    if (game_data.map->data[ix + game_data.map->width * ++iy] == 0) {
                        break;
                    }
                    if (game_data.map->data[ix + 1 + game_data.map->width * iy] != 0) {
                        break;
                    }
                    tmp_angle = vector_to_angle(ix - sx, iy - sy);
                    if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                        min_angle = tmp_angle;
                    }
                    if (map_raycast(tmp_angle, sx, sy).x != tile.x) {
                        break;
                    }
                }
                p1x = tile.pX;
                p1y = (int) (iy * game_data.tile_size);
                break;

            case map_tile_top: // down
                iy = (int) tile.y;
                iy = (int) tile.y - 1;

                // go left to get p2
                ix = (int) tile.x + 1;
                while (true) {
                    if (debug_shadow) {
                        fill_rectangle(ix * game_data.tile_size - 1, iy * game_data.tile_size - 1,
                                ix * game_data.tile_size + 1, iy * game_data.tile_size + 1);
                    }

                    if (game_data.map->data[--ix + game_data.map->width * iy] == 0) {
                        ix++;
                        break;
                    }
                    if (game_data.map->data[ix + game_data.map->width * (iy + 1)] != 0) {
                        ix++;
                        break;
                    }
                    tmp_angle = vector_to_angle(ix - sx, iy - sy);
                    if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                        min_angle = tmp_angle;
                    }
                    if (map_raycast(tmp_angle, sx, sy).y != tile.y) {
                        break;
                    }
                }
                p2x = (int) (ix * game_data.tile_size);
                p2y = tile.pY;

                // go right to get p1
                ix = (int) tile.x;
                while (true) {
                    if (debug_shadow) {
                        fill_rectangle(ix * game_data.tile_size - 1, iy * game_data.tile_size - 1,
                                ix * game_data.tile_size + 1, iy * game_data.tile_size + 1);
                    }

                    if (game_data.map->data[++ix + game_data.map->width * iy] == 0) {
                        break;
                    }
                    if (game_data.map->data[ix + game_data.map->width * (iy + 1)] != 0) {
                        break;
                    }
                    tmp_angle = vector_to_angle(ix - sx, iy - sy);
                    if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                        min_angle = tmp_angle;
                    }
                    if (map_raycast(tmp_angle, sx, sy).y != tile.y) {
                        break;
                    }
                }
                p1x = (int) (ix * game_data.tile_size);
                p1y = tile.pY;
                break;
        }
        min_angle += 0.01f;
        // System.out.println(tile.side + " : "
        // + (int)(sx * game_data.tile_size - mx + halfW) + ","
        // + (BERGS.height - (int)(sy * game_data.tile_size - my + halfH))
        // + " : " + p1x + "," + p1y + " : " + p2x + "," + p2y);
        // Code copied from
        // http://stackoverflow.com/questions/2049582/how-to-determine-a-point-in-a-triangle/14382692#14382692
        // (I should probably take the time to figure out exactly how this works)
        // Atimes2 = (-p1y * p2x + y * (-p1x + p2x) + x * (p1y - p2y) + p1x * p2y);
        // System.out.println(Atimes2);
        
        set_color_a(255, 0, 0, 60);
        fill_triangle(x, y, p1x, p1y, p2x, p2y);
        

//        maxY = min(RENDER.height, max(p0y, max(p2y, p1y)));
//        minY = max(0, min(p0y, min(p2y, p1y)));
//        maxX = min(RENDER.width, max(p0x, max(p2x, p1x)));
//        minX = max(0, min(p0x, min(p2x, p1x)));
//        for (int py = minY; py < maxY; py++) {
//            for (int px = minX; px < maxX; px++) {
//                s = 1 / Atimes2 * (p0y * p2x - p0x * p2y + (p2y - p0y) * px + (p0x - p2x) * py);
//                t = 1 / Atimes2 * (p0x * p1y - p0y * p1x + (p0y - p1y) * px + (p1x - p0x) * py);
//                // System.out.println(s + " " + t);
//                if (s > 0 && t > 0 && 1 - s - t > 0) {
//                    // System.out.println(px + "," + py);
//                    pixels[py * BERGS.width + px] = (byte) 0;
//                }
//            }
//        }
    }
    printf("iterations: %d\n", iter);
}