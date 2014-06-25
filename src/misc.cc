#include "misc.h"
#include <fstream>

const char* read_file(const std::string &filename, int *size) {
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        int length = file.tellg();
        if (size != NULL) {
            *size = length;
        }
        char *data = new char[length + 1];
        file.seekg(0, std::ios::beg);
        file.read(data, length);
        file.close();
        data[length] = '\0';
        return data;
    } else throw Exception("Failed to read file '" + filename + "'");
}
