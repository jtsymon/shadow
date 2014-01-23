
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
map_tile_collision map_raycast_ignore(double angle, double x, double y, int ignore_index) {

    int ix = (int) x, iy = (int) y;
    double dx = cos(angle);
    double dy = sin(angle);
    double middle = 0;
    int index;

    // printf("%f - %f,%f (%d,%d)\n", angle, x, y, ix, iy);

    // Work out which direction to go
    if (dx >= 0 && dy >= 0) {
        // Top right quadrant
        // printf("TopRight\n");

        while (ix < game_data.map->width && iy >= 0 && (game_data.map->data[index = ix + game_data.map->width * iy] == 0 ||
                index == ignore_index)) {

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

        while (ix >= 0 && iy >= 0 && (game_data.map->data[index = ix + game_data.map->width * iy] == 0 ||
                index == ignore_index)) {

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

        while (ix >= 0 && iy < game_data.map->height && (game_data.map->data[index = ix + game_data.map->width * iy] == 0 ||
                index == ignore_index)) {

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

        while (ix < game_data.map->width && iy < game_data.map->height && (game_data.map->data[index = ix + game_data.map->width * iy] == 0 ||
                index == ignore_index)) {

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

map_tile_collision map_raycast(double angle, double x, double y) {
    return map_raycast_ignore(angle, x, y, -1);
}

bool debug_shadow = true;
static double d = 0.001;

/**
 * Figure out which areas of the map are visible to the player
 * @param x centre x
 * @param y centre y
 */
void map_shadow(int x, int y) {
    double sx = (double) x / game_data.tile_size;
    double sy = (double) y / game_data.tile_size;
    // hackish fix for bug at round y values
    // TODO: fix this properly
    if(sy == (int)sy) sy += d;
    
    double min_angle = 0, tmp_angle = 0;
    map_tile_collision tile = map_raycast(min_angle, sx, sy);
    double fx = game_to_gl_x(tile.pX), fy = game_to_gl_y(tile.pY);
    bool halfway = false;
    int iter = 0;
    
    glColor4ub(255, 0, 0, 100);
    glBegin(GL_POLYGON);
    
    glVertex2d(game_to_gl_x(x), game_to_gl_y(y));
    glVertex2d(fx, fy);

    // unmask the regions that should be visible
    for (;;) {
        if(halfway) {
            if(min_angle < M_PI) break;
        } else if(min_angle > M_PI) {
            halfway = true;
        }
        if(iter > 1000) break;
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
            case map_tile_left:
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
        
        // printf("dX:%d,dY:%d,dX2:%d,dY2:%d\n", dX, dY, dX2, dY2);
        
        glVertex2d(game_to_gl_x(tile.pX), game_to_gl_y(tile.pY));
        
        // printf("start side:%d, x:%d, y:%d\n", tile.side, tile.x, tile.y);
        
        int ix = tile.x, iy = tile.y, side = tile.side;
        if(side == map_tile_left) {
            ix -= dX;
            iy -= dY;
        }
        
        bool avoid_tile = true;
        
        for(;;) {
            iy += dY;
            ix += dX;
            printf("%d,%d\n", ix, iy);
            
            // check if the wall ends or we reach a corner
            if (!game_data.map->data[ix + game_data.map->width * iy] ||
                    game_data.map->data[ix + dX2 + game_data.map->width * (iy + dY2)]) {
                iy -= dY;
                ix -= dX;
                switch(side) {
                    case map_tile_left: // go up
                        tile.pX = ix * game_data.tile_size;
                        tile.pY = iy * game_data.tile_size;
                        tmp_angle = vector_to_angle(ix - (d * dX) - sx, iy - (d * dY) - sy);
                        break;
                    case map_tile_right: // go down
                        tile.pX = (ix + 1) * game_data.tile_size;
                        tile.pY = (iy + 1) * game_data.tile_size;
                        tmp_angle = vector_to_angle(ix + 1 - (d * dX) - sx, iy + 1 - (d * dY) - sy);
                        break;
                    case map_tile_bottom: // go left
                        tile.pX = ix * game_data.tile_size;
                        tile.pY = (iy + 1) * game_data.tile_size;
                        tmp_angle = vector_to_angle(ix - (d * dX) - sx, iy + 1 - (d * dY) - sy);
                        break;
                    case map_tile_top: // go right
                        tile.pX = (ix + 1) * game_data.tile_size;
                        tile.pY = iy * game_data.tile_size;
                        tmp_angle = vector_to_angle(ix + 1 - (d * dX) - sx, iy - (d * dY) - sy);
                        break;
                }
                // skip the loop to fix position if we already fixed it
                map_tile_collision tmp_tile = map_raycast(tmp_angle, sx, sy);
                avoid_tile = (tmp_tile.side != side || (!dX && tmp_tile.x != ix) || (!dY && tmp_tile.y != iy));
                if(avoid_tile) {
                    tile = tmp_tile;
                }
                printf("end of wall\n");
                break;
            }
            tmp_angle = vector_to_angle(ix - (d * dX) - sx, iy - (d * dY) - sy);
            tile = map_raycast(tmp_angle, sx, sy);
            if(tile.side != side || (!dX && tile.x != ix) || (!dY && tile.y != iy)) {
                printf("angle:%d, side:%d, x:%d, y:%d\n", (int)(tmp_angle * 180 * M_1_PI), tile.side, tile.x, tile.y);
                printf("failed raycast\n");
                break;
            }
        }
        if(avoid_tile) {
            double aw, ah, arw, arh;
            int tile_index = tile.x + game_data.map->width * tile.y;
            int c = 0;
            // printf("tile side: %d, pos(%d,%d), correct side:%d, pos(%d,%d)\n", tile.side, tile.x, tile.y, side, ix, iy);
            while((!dX && tile.x != ix) || (!dY && tile.y != iy) || tile.side != side) {
                tile_index = tile.x + game_data.map->width * tile.y;
                if(c++ > 5) {
                    printf("failed to avoid tile\n");
                    break;
                }
                switch(tile.side) {
                    case map_tile_left: // go down
                        arw = tile.x - sx;
                        arh = tile.y + 1 - sy;
                        aw = arw;
                        ah = arh + d;
                        break;
                    case map_tile_right:  // go up
                        arw = tile.x + 1 - sx;
                        arh = tile.y - sy;
                        aw = arw;
                        ah = arh - d;
                        break;
                    case map_tile_bottom: // go right
                        arw = tile.x + 1 - sx;
                        arh = tile.y + 1 - sy;
                        aw = arw + d;
                        ah = arh;
                        break;
                    case map_tile_top:    // go left
                        arw = tile.x - sx;
                        arh = tile.y - sy;
                        aw = arw - d;
                        ah = arh;
                        break;
                }
                tmp_angle = vector_to_angle(aw, ah);
                tile = map_raycast(tmp_angle, sx, sy);
                // printf("angle:%d, side:%d, x:%d, y:%d\n", (int)(tmp_angle * 180 * M_1_PI), tile.side, tile.x, tile.y);
            }
            double tmp_tmp_angle = tmp_angle;
            tmp_angle = vector_to_angle(arw, arh);
            tile = map_raycast_ignore(tmp_angle, sx, sy, tile_index);
            // printf("FINAL angle:%d, side:%d, x:%d, y:%d\n", (int)(tmp_angle * 180 * M_1_PI), tile.side, tile.x, tile.y);
            tmp_angle = tmp_tmp_angle;
        }
        if(!halfway || tmp_angle > M_PI) {
            glVertex2d(game_to_gl_x(tile.pX), game_to_gl_y(tile.pY));
        }
        
        if (min_angle == tmp_angle) {
            // printf("No change to angle: %f\n", min_angle);
            min_angle += d;
        } else {
            min_angle = tmp_angle;
        }
//        switch(side) {
//            case map_tile_left: // go up
//                p2y -= game_data.tile_size / 8;
//                break;
//            case map_tile_right:  // go down
//                p2y += game_data.tile_size / 8;
//                break;
//            case map_tile_bottom: // go left
//                p2x -= game_data.tile_size / 8;
//                break;
//            case map_tile_top:    // go right
//                p2x += game_data.tile_size / 8;
//                break;
//        }
        
//        if (debug_shadow) {
//            glColor3ub(0, 255, 255); // cyan
//            fill_rectangle(p1x - 3, p1y - 3, p1x + 3, p1y + 3);
//            glColor3ub(255, 255, 0); // yellow
//            fill_rectangle(p2x - 3, p2y - 3, p2x + 3, p2y + 3);
//            
//            // printf("%d,%d,%d,%d,%d,%d\n", x, y, p1x, p1y, p2x, p2y);
//        }
//        if(dir == 1) {
//            glColor4ub(255, 0, 0, 100);
//        } else {
//            glColor4ub(0, 0, 255, 100);
//        }
    }
    
    glVertex2d(fx, fy);
    
    printf("iterations: %d\n", iter);
    
    glEnd();
}