/* 
 * File:   visibility_polygon.h
 * Author: jts
 *
 * Created on 18 February 2014, 3:10 PM
 */

#ifndef VISIBILITY_POLYGON_H
#define	VISIBILITY_POLYGON_H

typedef struct {
    int x1, y1,
        x2, y2;
} line_segment_t;

extern void map_line_segments(map_t* map);

#endif	/* VISIBILITY_POLYGON_H */

