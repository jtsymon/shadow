
#include "screens/game.h"
#include "map.h"

map_t* map_init(int width, int height, unsigned char data[]) {
    map_t *map = malloc(sizeof(int) * 2 + sizeof(unsigned char*));
    map->width = width;
    map->height = height;
    map->data = malloc(width * height);
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            map->data[x + width * y] = data[x + width * y];
        }
    }
    return map;
}

map_tile_collision map_raycast(double angle, double x, double y) {
    
    int ix = (int)x, iy = (int)y;
    double dx = cos(angle);
    double dy = sin(angle);
    double middle = 0;
    
    // printf("%f - %f,%f (%d,%d)\n", angle, x, y, ix, iy);
    
    // Work out which direction to go
    if(dx >= 0 && dy >= 0) {
        // Top right quadrant
        // printf("TopRight\n");

        while(ix < game_data.map->width && iy >= 0 && game_data.map->data[ix + game_data.map->width * iy] == 0) {

            middle = vector_to_angle((double)ix + 1 - x, (double)iy - y);
            
            // printf("%f (%f) - %f,%f (%d,%d)\n", middle, angle, x, y, ix, iy);

            if(angle < middle || angle == middle && angle == 0) {
                // exiting through right of tile
                ix++;
            } else {
                // exiting through top of tile
                iy--;
            }
        }

        // We hit square ix, iy, we can use last calculated middle to work out
        // which side was hit.
        if(angle < middle || angle == 0) {
            // Hit left side
            return (map_tile_collision) {
                ix, iy,
                game_data.map->data[ix + game_data.map->width * iy],
                map_tile_left,
                ix * game_data.tile_size,
                (y - dy * (ix - x) / dx) * game_data.tile_size
            };
        } else if(angle == middle) {
            // Hit the corner of a tile,
            // so we check the angle to that tile to figure out what side we hit
            middle = vector_to_angle((double)ix + 1 - x, (double)iy - y);
            if(angle <= middle) {
                if(angle == middle) {
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

    } else if(dx < 0 && dy >= 0) {
        // Top left quadrant
        // printf("TopLeft\n");
        
        if(ix == x) {
            ix--;
        }

        while(ix >= 0 && iy >= 0 && game_data.map->data[ix + game_data.map->width * iy] == 0) {

            middle = vector_to_angle((double)ix - x, (double)iy - y);
            
            // printf("%f (%f) - %f,%f (%d,%d)\n", middle, angle, x, y, ix, iy);

            if(angle < middle || angle == middle && angle == M_PI) {
                // exiting through top of tile
                iy--;
            } else {
                // exiting through left of tile
                ix--;
            }
        }

        // We hit square ix, iy, we can use last calculated middle to work out
        // which side was hit.
        if(angle == middle) {
            // Hit the corner of a tile,
            // so we check the angle to that tile to figure out what side we hit
            middle = vector_to_angle((double)ix - x, (double)iy - y);
            if(angle >= middle) {
                if(angle == middle) {
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
        } else if(angle > middle) {
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

    } else if(dx < 0 && dy < 0) {
        // Bottom left quadrant
        // printf("BottomLeft\n");

        if(ix == x) {
            ix--;
        }

        while(ix >= 0 && iy < game_data.map->height && game_data.map->data[ix + game_data.map->width * iy] == 0) {

            middle = vector_to_angle((double)ix - x, (double)iy + 1 - y);
            
            // printf("%f (%f) - %f,%f (%d,%d)\n", middle, angle, x, y, ix, iy);

            if(angle < middle || angle == middle && angle == M_PI_2 * 3) {
                // exiting through left of tile
                ix--;
            } else {
                // exiting through bottom of tile
                iy++;
            }
        }

        // We hit square ix, iy, we can use last calculated middle to work out
        // which side was hit.
        if(angle == middle) {
            // Hit the corner of a tile,
            // so we check the angle to that tile to figure out what side we hit
            middle = vector_to_angle((double)ix - x, (double)iy + 1 - y);
            if(angle >= middle) {
                if(angle == middle) {
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
        } else if(angle > middle) {
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

    } else if(dx >= 0 && dy < 0) {
        // Bottom right quadrant
        // printf("BottomRight\n");

        while(ix < game_data.map->width && iy < game_data.map->height && game_data.map->data[ix + game_data.map->width * iy] == 0) {

            middle = vector_to_angle((double)ix + 1 - x, (double)iy + 1 - y);
            
            // printf("%f (%f) - %f,%f (%d,%d)\n", middle, angle, x, y, ix, iy);

            if(angle < middle || angle == middle && angle == M_PI * 2) {
                // exiting through bottom of tile
                iy++;
                
            } else {
                // exiting through right of tile
                ix++;
            }
        }

        // We hit square ix, iy, we can use last calculated middle to work out
        // which side was hit.
        if(angle == middle) {
            printf("YOLO SWAG\n");
            // Hit the corner of a tile,
            // so we check the angle to that tile to figure out what side we hit
            middle = vector_to_angle((double)ix - x, (double)iy + 1 - y);
            if(angle >= middle) {
                if(angle == middle) {
                    // this shouldn't happen
                    printf("Error calculating angle: %f - %d,%d\n", angle, ix, iy);
                }
                // Hit top side
                printf("top\n");
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
        } else if(angle > middle) {
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