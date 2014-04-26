#include "misc.h"

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