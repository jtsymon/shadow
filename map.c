
#include <sys/stat.h>

#include "map.h"
#include "screens/game.h"

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
    char* data = read_file(filename);
    if(!data) {
        return NULL;
    }

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
    printf("loaded %s\n", filename);
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
                //iy++;

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_bottom,
                            ix * game_data.tile_size,
                            (iy + 1) * game_data.tile_size
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
            //iy++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_bottom,
                        (x - dx * (iy + 1 - y) / dy) * game_data.tile_size,
                        (iy + 1) * game_data.tile_size
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
                //iy++;

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_bottom,
                            ix * game_data.tile_size,
                            (iy + 1) * game_data.tile_size
                };
            } else {
                // Hit right side
                //ix++;

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_right,
                            (ix + 1) * game_data.tile_size,
                            iy * game_data.tile_size
                };
            }
        } else if (angle > middle) {
            // Hit right side
            //ix++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_right,
                        (ix + 1) * game_data.tile_size,
                        (y - dy * (ix + 1 - x) / dx) * game_data.tile_size
            };
        } else {
            // Hit bottom
            //iy++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_bottom,
                        (x - dx * (iy + 1 - y) / dy) * game_data.tile_size,
                        (iy + 1) * game_data.tile_size
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
                //ix++;

                return (map_tile_collision) {
                    ix, iy,
                            game_data.map->data[ix + game_data.map->width * iy],
                            map_tile_right,
                            (ix + 1) * game_data.tile_size,
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
            //ix++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_right,
                        (ix + 1) * game_data.tile_size,
                        (y - dy * (ix + 1 - x) / dx) * game_data.tile_size
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
                        map_tile_left,
                        ix * game_data.tile_size,
                        (y - dy * (ix - x) / dx) * game_data.tile_size
            };
        } else {
            // Hit top side
            // ix++;

            return (map_tile_collision) {
                ix, iy,
                        game_data.map->data[ix + game_data.map->width * iy],
                        map_tile_top,
                        (x - dx * (iy - y) / dy) * game_data.tile_size,
                        iy * game_data.tile_size
            };
        }

    } // End of finding square collision.

}

bool debug_shadow = true;
static double d = 0.001;

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
        
        // printf("%d:%d %d:%d", tile.x, ix, tile.y, iy);
        
        // direction to move in
        int dX = 0, dY = 0;
        // direction to check in
        int dX2 = 0, dY2 = 0;
        
        switch (tile.side) {
            case map_tile_left: // right
                dY = -1;
                dX2 = -1;
                break;
            case map_tile_right:
                dY = 1;
                dX2 = 1;
                break;
            case map_tile_top:
                dX = 1;
                dY2 = -1;
                break;
            case map_tile_bottom:
                dX = -1;
                dY2 = 1;
                break;
        }
        
        printf("dX:%d,dY:%d,dX2:%d,dY2:%d\n", dX, dY, dX2, dY2);
        
        p1x = tile.pX;
        p1y = tile.pY;
        
        printf("start side:%d, x:%d, y:%d\n", tile.side, tile.x, tile.y);
        
        int ix = tile.x, iy = tile.y, side = tile.side;
        
        bool changed_angle = false;
        
        while (true) {
            iy += dY;
            ix += dX;
            printf("%d,%d\n", ix, iy);
            
            tmp_angle = vector_to_angle(ix - (d * dX) - sx, iy - (d * dY) - sy);
            if (tmp_angle > min_angle && tmp_angle < min_angle + M_PI) {
                min_angle = tmp_angle;
                changed_angle = true;
            }
            tile = map_raycast(tmp_angle, sx, sy);
            if(tile.side != side || (!dX && tile.x != ix) || (!dY && tile.y != iy)) {
                printf("angle:%d, side:%d, x:%d, y:%d\n", (int)(tmp_angle * 180 * M_1_PI), tile.side, tile.x, tile.y);
                printf("failed raycast\n");
                break;
            }
            
            // check if the wall ends or we reach a corner
            if (!game_data.map->data[ix + game_data.map->width * iy] ||
                    game_data.map->data[ix + dX2 + game_data.map->width * (iy + dY2)]) {
                iy -= dY;
                ix -= dX;
                switch(tile.side) {
                    case map_tile_left:
                        tile.pX = ix * game_data.tile_size;
                        tile.pY = iy * game_data.tile_size;
                        break;
                    case map_tile_right:
                        tile.pX = (ix + 1) * game_data.tile_size;
                        tile.pY = (iy + 1) * game_data.tile_size;
                        break;
                    case map_tile_bottom:
                        tile.pX = ix * game_data.tile_size;
                        tile.pY = (iy + 1) * game_data.tile_size;
                        break;
                    case map_tile_top:    // go left
                        tile.pX = (ix + 1) * game_data.tile_size;
                        tile.pY = iy * game_data.tile_size;
                        break;
                }
                tile.side = side; // skip the loop to fix position, since we already fixed it
                printf("end of wall\n");
                break;
            }
        }
        int c = 0;
        while((!dX && tile.x != ix) || (!dY && tile.y != iy) || tile.side != side) {
            if(c++ > 5) break;
            switch(tile.side) {
                case map_tile_left: // go down
                    // printf("left\n");
                    tmp_angle = vector_to_angle(tile.x - sx, tile.y + 1 + d - sy);
                    break;
                case map_tile_right:  // go up
                    // printf("right\n");
                    tmp_angle = vector_to_angle(tile.x + 1 - sx, tile.y - d - sy);
                    break;
                case map_tile_bottom: // go right
                    // printf("bottom\n");
                    tmp_angle = vector_to_angle(tile.x + 1 + d - sx, tile.y + 1 - sy);
                    break;
                case map_tile_top:    // go left
                    // printf("top\n");
                    tmp_angle = vector_to_angle(tile.x - d - sx, tile.y - sy);
                    break;
            }
            tile = map_raycast(tmp_angle, sx, sy);
        }
        
        p2x = tile.pX;
        p2y = tile.pY;

        if(!changed_angle) min_angle += 0.01f;
        
        if (debug_shadow) {
            glColor3ub(0, 255, 255);
            fill_rectangle(p1x - 3, p1y - 3, p1x + 3, p1y + 3);
            glColor3ub(0, 255, 255);
            fill_rectangle(p2x - 3, p2y - 3, p2x + 3, p2y + 3);
            
            printf("%d,%d,%d,%d,%d,%d\n", x, y, p1x, p1y, p2x, p2y);
        }
        
        glColor4ub(255, 0, 0, 60);
        fill_triangle(x, y, p1x, p1y, p2x, p2y);
    }
    //printf("iterations: %d\n", iter);
}