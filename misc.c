#include "misc.h"

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

int max(int a, int b) {
    return a > b ? a : b;
}
int min(int a, int b) {
    return a < b ? a : b;
}
