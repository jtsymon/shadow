#include "map.h"

#include <sys/stat.h>
#include <stdio.h>
#include <cmath>
#include <list>
#include <fstream>
#include <iostream>

#include "../main.h"
#include "../math/vector.h"
#include "../math/math.h"

Vector<int> Map::point_read(const std::string &line) {
    char *buf = (char*) line.c_str();
    if (*buf > '9' || *buf < '0') throw Exception("Invalid point encountered: " + line);
    // advance to end of number
    const char *x = buf;
    while (*buf <= '9' && *buf >= '0') ++buf;
    *buf++ = '\0';
    // advance to next number
    while (*buf == ' ' || *buf == '\t') ++buf;
    if (*buf > '9' || *buf < '0') throw Exception("Invalid point encountered: " + line);
    // advance to end of number
    const char *y = buf;
    while (*buf <= '9' && *buf >= '0') ++buf;
    if (*buf == '\n' || *buf == ' ') *buf = '\0';
    else if (*buf) throw Exception("Invalid point encountered: " + line);

    Vector<int> point(atoi(x), atoi(y));
    std::cout << "Point: " << point.x << "," << point.y << std::endl;

    return point;
}

MapSegment Map::segment_read(const std::string &line) {
    char *buf = (char*) line.c_str();
    if (*buf > '9' || *buf < '0') throw Exception("Invalid line segment encountered: " + line);
    // advance to end of number
    const char *a = buf;
    while (*buf <= '9' && *buf >= '0') ++buf;
    *buf++ = '\0';
    // advance to next number
    while (*buf == ' ' || *buf == '\t') ++buf;
    if (*buf > '9' || *buf < '0') throw Exception("Invalid line segment encountered: " + line);
    // advance to end of number
    const char *b = buf;
    while (*buf <= '9' && *buf >= '0') ++buf;
    if (*buf == '\n' || *buf == ' ') *buf = '\0';
    else if (*buf) throw Exception("Invalid line segment encountered: " + line);

    MapSegment segment(atoi(a), atoi(b));

    std::cout << "Line Segment: " << segment.a << " (" <<
            this->points[segment.a].x << "," << this->points[segment.a].y <<
            "), " << segment.b << " (" << this->points[segment.b].x << "," <<
            this->points[segment.b].y << ")" << std::endl;

    return segment;
}

std::vector<int> Map::polygon_read(const std::string &line) {
    char *buf = (char*) line.c_str();
    std::vector<int> polygon;
    for (;;) {
        // advance to next number
        while (*buf == ' ' || *buf == '\t') ++buf;
        if (*buf == '\0' || *buf == '\n') break;
        if (*buf > '9' || *buf < '0') throw Exception("Invalid polygon encountered: " + line);
        // advance to end of number
        const char *id = buf;
        while (*buf <= '9' && *buf >= '0') ++buf;
        if (*buf) *buf++ = '\0';
        polygon.push_back(atoi(id));
    }

    printf("Polygon with %d line segments\n", polygon.size());

    return polygon;
}

Map::Map(const std::string &filename) : mask(width, height) {
    std::ifstream file(filename.c_str(), std::ios::in);
    if (!file.is_open()) {
        throw Exception("Failed to open " + filename);
    }
    std::string line;
    // read points
    while (file.good()) {
        std::getline(file, line);
        if (line[0] == '\0' || line[0] == '\n') {
            break;
        }
        try {
            this->points.push_back(point_read(line));
        } catch (Exception e) {
            std::cerr << e.what() << std::endl;
        }
    }
    // read line segments
    while (file.good()) {
        std::getline(file, line);
        if (line[0] == '\0' || line[0] == '\n') {
            break;
        }
        try {
            this->segments.push_back(segment_read(line));
        } catch (Exception e) {
            std::cerr << e.what() << std::endl;
        }
    }
    // read polygons
    while (file.good()) {
        std::getline(file, line);
        if (line[0] == '\0' || line[0] == '\n') {
            break;
        }
        try {
            this->polygons.push_back(polygon_read(line));
        } catch (Exception e) {
            std::cerr << e.what() << std::endl;
        }
    }
    std::cout << "loaded " << filename << std::endl;
}

#define NoCollision RayCollision(0, 0, INFINITY)

RayCollision __ray_intersect(Vector<int> p, double m, double c,
        double cosa, double sina, Vector<int> s1, Vector<int> s2) {

    int min_x = std::min(s1.x, s2.x);
    int max_x = std::max(s1.x, s2.x);
    int min_y = std::min(s1.y, s2.y);
    int max_y = std::max(s1.y, s2.y);

    // vertical line segment case
    if (s1.x == s2.x) {
        if (p.x <= s1.x && cosa <= 0 || p.x > s1.x && cosa > 0) return NoCollision;
        double ey = c + s1.x * m;
        if (ey >= min_y && ey <= max_y) {
            double dy = ey - p.y;
            double dx = s1.x - p.x;
            double dist = std::sqrt(dy * dy + dx * dx);

            return RayCollision(s1.x, ey, dist);
        }
        return NoCollision;
    }
    // calculate the gradient now that we know it's non-zero
    double sm = (double) (s2.y - s1.y) / (s2.x - s1.x);
    double sc = s1.y - sm * s1.x;

    // ray starts on the line case
    if (float_equal((double) p.y, sm * p.x + sc) && float_equal((double) p.y, m * p.x + c)
            && p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y) {
        // printf("p.y=%d=%d=%d, p.x=%d, min_x=%d, max_x=%d, min_y=%d, max_y=%d\n", p.y, sm * p.x + sc, m * p.x + c, p.x, min_x, max_x, min_y, max_y);

        return RayCollision(p.x, p.y, 0);
    }
    // parallel lines case
    if (float_equal(m, sm)) {
        // check if they're from the same line
        if (!float_equal(c, sc)) return NoCollision;
        if (p.x >= min_x && p.x <= max_x) {
            if (p.y >= min_y && p.y <= max_y) return RayCollision(p.x, p.y, 0);
            if (p.y <= min_y && sina < 0) return RayCollision(p.x, min_y, min_y - p.y);
            if (p.y >= max_y && sina > 0) return RayCollision(p.x, max_y, p.y - max_y);
        }
        if (p.x <= min_x && cosa > 0) {
            if (p.y >= min_y && p.y <= max_y) return RayCollision(min_x, p.y, min_x - p.x);
            if (p.y <= min_y && sina < 0) {
                double dx = min_x - p.x;
                double dy = min_y - p.y;
                double dist = sqrt(dy * dy + dx * dx);

                return RayCollision(min_x, min_y, dist);
            }
            if (p.y >= max_y && sina > 0) {
                double dx = min_x - p.x;
                double dy = p.y - max_y;
                double dist = sqrt(dy * dy + dx * dx);

                return RayCollision(min_x, max_y, dist);
            }
        }
        if (p.x >= max_x && cosa < 0) {
            if (p.y >= min_y && p.y <= max_y) return RayCollision(max_x, p.y, p.x - max_x);
            if (p.y <= min_y && sina < 0) {
                double dx = p.x - max_x;
                double dy = min_y - p.y;
                double dist = sqrt(dy * dy + dx * dx);

                return RayCollision(max_x, min_y, dist);
            }
            if (p.y >= max_y && sina > 0) {
                double dx = p.x - max_x;
                double dy = p.y - max_y;
                double dist = sqrt(dy * dy + dx * dx);

                return RayCollision(max_x, max_y, dist);
            }
        }

        return NoCollision;
    }
    // standard case
    double ex = (sc - c) / (m - sm);
    if (ex < min_x - M_DELTA || ex > max_x + M_DELTA ||
            ex < p.x && cosa > 0 || ex > p.x && cosa < 0) {
        return NoCollision;
    }
    double ey = c + ex * m;
    if (ey < min_y - M_DELTA || ey > max_y + M_DELTA ||
            ey < p.y && sina > 0 || ey > p.y && sina < 0) {
        return NoCollision;
    }
    double dx = ex - p.x;
    double dy = ey - p.y;
    double dist = sqrt(dx * dx + dy * dy);

    return RayCollision(ex, ey, dist);
}

RayCollision __ray_intersect_v(Vector<int> p, double sina, Vector<int> s1, Vector<int> s2) {
    // parallel lines (vertical line segment) case

    int min_x = std::min(s1.x, s2.x);
    int max_x = std::max(s1.x, s2.x);
    int min_y = std::min(s1.y, s2.y);
    int max_y = std::max(s1.y, s2.y);

    if (s2.x == s1.x) {
        if (p.x != s1.x) return NoCollision;
        if (p.y >= min_y && p.y <= max_y) return RayCollision(p.x, p.y, 0);
        if (p.y <= min_y && sina > 0) return RayCollision(p.x, min_y, min_y - p.y);
        if (p.y >= max_y && sina < 0) return RayCollision(p.x, max_y, p.y - max_y);
        return NoCollision;
    }
    // calculate the gradient now that we know it's non-zero
    double sm = (double) (s2.y - s1.y) / (s2.x - s1.x);
    double sc = s1.y - sm * s1.x;
    // ray starts on the line case
    if (float_equal((double) p.y, sm * p.x + sc) &&
            p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y) {
        // printf("p.y=%d=%d, p.x=%d, min_x=%d, max_x=%d, min_y=%d, max_y=%d\n", p.y, sm * p.x + sc, min_x, max_x, min_y, max_y);

        return RayCollision(p.x, p.y, 0);
    }
    // standard case
    if (p.x >= min_x && p.x <= max_x) {
        double ey = sc + p.x * sm;
        if (p.y >= ey && sina < 0 || p.y <= ey && sina > 0) {

            return RayCollision(p.x, ey, std::abs(p.y - ey));
        }
    }
    return NoCollision;
}

RayCollision Map::__raycast(Vector<int> p, double m, double c, double cosa, double sina) {
    RayCollision collision = NoCollision;
    for (MapSegment wall : this->segments) {

        RayCollision new_collision = __ray_intersect(p, m, c, cosa, sina,
                Vector<int>(this->points[wall.a].x, this->points[wall.a].y),
                Vector<int>(this->points[wall.b].x, this->points[wall.b].y));
        //            if(new_collision.dist != INFINITY) {
        //                printf("%f, %f, %f\n", new_collision.x, new_collision.y, new_collision.dist);
        //            }
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    }
    return collision;
}

RayCollision Map::__raycast_v(Vector<int> p, double sina) {
    RayCollision collision = NoCollision;
    for (MapSegment wall : this->segments) {

        RayCollision new_collision = __ray_intersect_v(p, sina,
                Vector<int>(this->points[wall.a].x, this->points[wall.a].y),
                Vector<int>(this->points[wall.b].x, this->points[wall.b].y));
        //            if(new_collision.dist != INFINITY) {
        //                printf("%f, %f, %f\n", new_collision.x, new_collision.y, new_collision.dist);
        //            }
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    }
    return collision;
}

RayCollision Map::raycast(Vector<int> p, double angle) {

    double cosa = cos(angle);
    double sina = -sin(angle);

    if (std::abs(cosa) > M_DELTA) {
        double m = sina / cosa;
        double c = p.y - m * p.x;
        return __raycast(p, m, c, cosa, sina);
    } else {
        return __raycast_v(p, sina);
    }
}

RayCollision Map::shadow_raycast(Vector<int> p, double angle) {
    double cosa = cos(angle);
    double sina = -sin(angle);
    RayCollision collision(0, 0, INFINITY);

    if (std::abs(cosa) > M_DELTA) {
        double m = sina / cosa;
        double c = p.y - m * p.x;
        // check edges of the map
        // top
        RayCollision new_collision = __ray_intersect(p, m, c, cosa, sina,
                Vector<int>(0, 0), Vector<int>(width, 0));
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // right
        new_collision = __ray_intersect(p, m, c, cosa, sina,
                Vector<int>(width, 0), Vector<int>(width, height));
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // bottom
        new_collision = __ray_intersect(p, m, c, cosa, sina,
                Vector<int>(width, height), Vector<int>(0, height));
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // left
        new_collision = __ray_intersect(p, m, c, cosa, sina,
                Vector<int>(0, height), Vector<int>(0, 0));
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        new_collision = this->__raycast(p, m, c, cosa, sina);
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    } else {
        // check edges of the map
        // top
        RayCollision new_collision = __ray_intersect_v(p, sina,
                Vector<int>(0, 0), Vector<int>(width, 0));
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // right
        new_collision = __ray_intersect_v(p, sina,
                Vector<int>(width, 0), Vector<int>(width, height));
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // bottom
        new_collision = __ray_intersect_v(p, sina,
                Vector<int>(width, height), Vector<int>(0, height));
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        // left
        new_collision = __ray_intersect_v(p, sina,
                Vector<int>(0, height), Vector<int>(0, 0));
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
        new_collision = this->__raycast_v(p, sina);
        if (new_collision.dist < collision.dist) {
            collision = new_collision;
        }
    }
    return collision;
}

static bool sorter(double first, double second) {
    return first >= second;
}

void Map::shadow(Vector<int> p) {
    // draw to the framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER, RENDER.mask.framebuffer);
    // glClearColor(255, 0, 0, 255);

    int i;
    std::list<double> angles;
    for (Vector<int> point : this->points) {
        double angle = atan2(p.y - point.y, point.x - p.x);
        angles.push_back(angle_sanify(angle - SHADOW_DELTA));
        angles.push_back(angle_sanify(angle));
        angles.push_back(angle_sanify(angle + SHADOW_DELTA));
    }
    // check edges of the screen
    angles.push_back(angle_sanify(atan2(p.y, -p.x)));
    angles.push_back(angle_sanify(atan2(p.y, width - p.x)));
    angles.push_back(angle_sanify(atan2(p.y - height, width - p.x)));
    angles.push_back(angle_sanify(atan2(p.y - height, -p.x)));

    // sort the list clockwise
    angles.sort(sorter);

    int size = angles.size() + 3;
    // TODO: Maybe use a Batch once I implement parameter handling -- not practical yet
    // Batch shadow_batch(GL_TRIANGLE_FAN, RGBA colour, GLuint shader, int size=1024)
    GLfloat data[size * 2];
    i = 0;

    data[i++] = game_to_gl_x(p.x);
    data[i++] = game_to_gl_y(p.y);

    RayCollision first = this->shadow_raycast(p, angles.front());
    data[i++] = game_to_gl_x(first.x);
    data[i++] = game_to_gl_y(first.y);
    angles.pop_front();
    for (double angle : angles) {
        RayCollision collision = this->shadow_raycast(p, angle);
        data[i++] = game_to_gl_x(collision.x);
        data[i++] = game_to_gl_y(collision.y);
    }

    data[i++] = game_to_gl_x(first.x);
    data[i++] = game_to_gl_y(first.y);
    data[i++] = game_to_gl_x(p.x);
    data[i++] = game_to_gl_y(p.y);

    // Draw shadow mask
    mask.begin();
    
    // background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(Graphics::shaders[GRAPHICS_SHADOW_SHADER]);

    glBindVertexArray(Graphics::vertex_array[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
    glVertexAttribPointer(
            0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
            2, // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            NULL // array buffer offset
            );
    glDrawArrays(GL_TRIANGLE_FAN, 0, size);
    glDisableVertexAttribArray(0);
    
    glUseProgram(0);
    
    mask.end();
    
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    
    mask.draw(RGBA(255, 0, 0, 255), 0, 0, width, height);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
