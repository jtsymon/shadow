#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

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

double mind(double a, double b) {
    return a < b ? a : b;
}

double maxd(double a, double b) {
    return a > b ? a : b;
}

double absd(double d) {
    return d >= 0 ? d : -d;
}

int equald(double a, double b) {
    return absd(a - b) < delta;
}

double angle_sanify(double a) {
    if(a < 0) a += (1 + (int)(a / (M_PI * 2))) * M_PI * 2;
    return a;
}

/**
 * Reads a file and returns the data from the file
 * The receiver is responsible for free()ing the returned char* array
 */
char* read_file(const char* filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        fprintf(stderr, "Cannot determine size of %s: %s\n", filename, strerror(errno));
        return NULL;
    }
    int file_size = st.st_size;

    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Cannot open %s for binary reading: %s\n", filename, strerror(errno));
        return NULL;
    }

    char* data = malloc(file_size + 1);
    fread(data, 1, file_size, file);
    fclose(file);
    data[file_size] = '\0';
    
    return data;
}