/* 
 * File:   vector.h
 * Author: jts
 *
 * Created on 25 April 2014, 1:59 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include <stdint.h>
#include "math.h"

template <class T>
class Vector {
public:
    T x, y;

    Vector(T x, T y) : x(x), y(y) {
    }
    double dist(Vector<T> v);
    T dist_sq(Vector<T> v);
    double magnitude();
    T dot(Vector v);
    Vector<T> scale(T factor);
    Vector<T> add(Vector<T> v);
    Vector<T> sub(Vector<T> v);
    Vector<T> normalise();
    Vector<int> toInt();
    Vector<double> toDouble();
    double angle();
};

template <class T>
class LineSegment {
public:
    Vector<T> v1, v2;

    LineSegment(Vector<T> v1, Vector<T> v2) : v1(v1), v2(v2) {
    }

    int side(Vector<T> v);
    double dist(Vector<T> p, bool &endpoint);
    Vector<double> normal(int side);
};

template <class T>
double Vector<T>::dist(Vector<T> v) {
    double dx = this->x - v.x;
    double dy = this->y - v.y;
    return sqrt((double)(dx * dx + dy * dy));
}

template <class T>
T Vector<T>::dist_sq(Vector<T> v) {
    double dx = this->x - v.x;
    double dy = this->y - v.y;
    return dx * dx + dy * dy;
}

template <class T>
double Vector<T>::magnitude() {
    return std::sqrt((double)(this->x * this->x + this->y * this->y));
}

template <class T>
T Vector<T>::dot(Vector<T> v) {
    return this->x * v.x + this->y * v.y;
}

template <class T>
Vector<T> Vector<T>::scale(T factor) {
    return Vector<T>(this->x * factor, this->y * factor);
}

template <class T>
Vector<T> Vector<T>::add(Vector<T> v) {

    return Vector<T>(this->x + v.x, this->y + v.y);
}

template <class T>
Vector<T> Vector<T>::sub(Vector<T> v) {

    return Vector<T>(this->x - v.x, this->y - v.y);
}

template <class T>
double Vector<T>::angle() {
    return std::atan2((double)this->y, (double)this->x);
}

template <class T>
Vector<T> Vector<T>::normalise() {
    return this->scale(1.0 / std::abs(this->magnitude()));
}

template <class T>
Vector<int> Vector<T>::toInt() {
    return Vector<int>((int) this->x, (int) this->y);
}

template <class T>
Vector<double> Vector<T>::toDouble() {
    return Vector<double>((double) this->x, (double) this->y);
}

template <class T>
int LineSegment<T>::side(Vector<T> v) {
    return sign((this->v2.x - this->v1.x) * (v.y - this->v1.y) - (this->v2.y - this->v1.y) * (v.x - this->v1.x));
    // return sign((w.x - v.x) * (p.y - v.y) - (w.y - v.y) * (p.x - v.x));
}

template <class T>
double LineSegment<T>::dist(Vector<T> p, bool &endpoint) {
    // Return minimum distance between line segment (v1, v2) and point p
    endpoint = true;
    double l2 = this->v1.dist_sq(this->v2); // i.e. |v2-v1|^2 -  avoid a sqrt
    if (l2 == 0.0) {
        return p.dist(this->v1); // v1 == v2 case
    }
    // Consider the line extending the segment, parameterized as v1 + t (v2 - v1).
    // We find projection of point p onto the line. 
    // It falls where t = [(p - v1) . (v2 - v1)] / |v2 - v1|^2
    double t = p.sub(v1).dot(this->v2.sub(v1)) / l2;
    if (t < 0.0) {
        return p.dist(v1); // Beyond the 'v1' end of the segment
    } else if (t > 1.0) {
        return p.dist(v2); // Beyond the 'v2' end of the segment
    }
    Vector<double> projection = this->v1.toDouble().add(this->v2.sub(this->v1).toDouble().scale(t)); // Projection falls on the segment
    endpoint = false;
    return p.toDouble().dist(projection);
}

template <class T>
Vector<double> LineSegment<T>::normal(int side) {
    if (side == 1) {
        return Vector<double>((double) (this->v1.y - this->v2.y), (double) (this->v2.x - this->v1.x));
    } else {
        return Vector<double>((double) (this->v2.y - this->v1.y), (double) (this->v1.x - this->v2.x));
    }
}

#endif	/* VECTOR_H */
