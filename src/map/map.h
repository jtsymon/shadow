/* 
 * File:   map.h
 * Author: jts
 *
 * Created on 11 January 2014, 1:37 PM
 */

#ifndef MAP_H
#define	MAP_H

#include "../misc.h"
#include "../graphics/Shaders.h"
#include "../graphics/Buffer.h"
#include "../math/vector.h"
#include <vector>
#include <string>

/*
 * Map Format:
 * 
 * List of points:
 * x,y
 * x,y
 * x,y
 * 
 * List of line formed from points:
 * pointid:pointid
 * pointid:pointid
 * pointid:pointid
 * 
 * List of polygons:
 * segmentid,segmentid,segmentid:texture
 * segmentid,segmentid,segmentid,segmentid:texture
 */

class MapSegment {
public:
    // indices into the point array
    int a, b;

    MapSegment(int a, int b) : a(a), b(b) {
    }
};

class RayCollision {
public:
    int x, y;
    double dist;

    RayCollision(int x, int y, double dist) : x(x), y(y), dist(dist) {
    }
};

//    class ShadowMask {
//        GLuint texture;
//        GLuint framebuffer;
//        GLuint vertex_buffer;
//        GLuint vertex_array;
//        Program program;
//        ShadowMask() : program(Program("shaders/pass_through_texture.vert", "shaders/shadow_mask.frag")) { }
//    };

class Map {
    Vector<int> point_read(const std::string &line);
    MapSegment segment_read(const std::string &line);
    std::vector<int> polygon_read(const std::string &line);
    RayCollision __raycast(Vector<int> p, double m, double c, double cosa, double sina);
    RayCollision __raycast_v(Vector<int> p, double sina);
    RayCollision shadow_raycast(Vector<int> p, double angle);
    
    Buffer mask;

public:
    // unique endpoints of line_segments
    std::vector<Vector<int>> points;
    // line segments made up from points
    std::vector<MapSegment> segments;
    // combinations of line segments, used for drawing wall textures
    std::vector<std::vector<int>> polygons;

    Map(const std::string &name);
    RayCollision raycast(Vector<int> p, double angle);
    void shadow(Vector<int> p);

    // static ShadowMask shadow_mask;
};


#endif	/* MAP_H */

