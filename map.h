/* 
 * File:   map.h
 * Author: jts
 *
 * Created on 11 January 2014, 1:37 PM
 */

#include <stdlib.h>

#ifndef map_h
#define	map_h

typedef struct {
    int width, height;
    
    unsigned char* data;

} map_t;

extern map_t* map_init();

#endif	/* map_h */

