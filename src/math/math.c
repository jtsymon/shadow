
#include "math.h"

double vector_to_angle(double w, double h) {
    
    // printf("w=%f, h=%f\n", w, h);
    
    if(w == 0 && h == 0) {
        return 0;
    }
    if(w == 0) {
        return h > 0 ? M_PI_2 : M_PI_2 * 3;
    }
    double ret = -atan(h / w);
    if(w < 0) {
        ret += M_PI;
    }
    if(ret < 0) {
        ret += 2 * M_PI;
    }
    return ret;
}

inline int max(int a, int b) {
    return a > b ? a : b;
}
inline int min(int a, int b) {
    return a < b ? a : b;
}

inline double mind(double a, double b) {
    return a < b ? a : b;
}

inline float minf(float a, float b) {
    return a < b ? a : b;
}

inline double maxd(double a, double b) {
    return a > b ? a : b;
}

inline float maxf(float a, float b) {
    return a > b ? a : b;
}

inline double absd(double d) {
    return d >= 0 ? d : -d;
}

inline float absf(float f) {
    return f >= 0 ? f : -f;
}

inline bool equald(double a, double b) {
    return absd(a - b) < M_DELTA;
}

inline bool equalf(float a, float b) {
    return absf(a - b) < M_DELTA;
}

inline int sign(double a) {
    return a > 0 ? 1 : a < 0 ? -1 : 0;
}

inline double angle_sanify(double a) {
    if(a < 0) a += (1 + (int)(a / (M_PI * 2))) * M_PI * 2;
    return a;
}

inline float angle_sanifyf(float a) {
    if(a < 0) a += (1 + (int)(a / (M_PI * 2))) * M_PI * 2;
    return a;
}
