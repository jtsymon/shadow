
#include "font.h"

bool font_load_fnt(font_t* font, char* filename, char* extension) {
    extension[0] = 'f';
    extension[1] = 'n';
    extension[2] = 't';
    
    // Load font info
    unsigned char* data = read_file(filename);
    if(!data) {
        if(filename) free(filename);
        return false;
    }
    
    int p = 0;
    font->min_char  = data[p++];
    font->max_char  = data[p++];
    font->tex_size  = pow(2, data[p++]);
    font->h         = data[p++];
    int num_chars   = font->max_char - font->min_char;
    font->chars     = malloc(num_chars * 4 * sizeof(GLfloat));
    font->w         = malloc(num_chars * sizeof(int));
    
    int i;
    for(i = 0; i < num_chars; i++) {
        int x = data[p++];
        int y = data[p++];
        font->w[i] = data[p++];
        int c = i * 4;
        font->chars[c++] = (GLfloat)x / font->tex_size;
        font->chars[c++] = (GLfloat)y / font->tex_size;
        font->chars[c++] = (GLfloat)(x + font->w[i]) / font->tex_size;
        font->chars[c++] = (GLfloat)(y + font->h) / font->tex_size;
    }
    
    free(data);
    
    printf("loaded %s\n", filename);
    return true;
}

int font_load_img(font_t* font, char* filename, char* extension) {
    extension[0] = 'p';
    extension[1] = 'n';
    extension[2] = 'g';
    
    // Load font bitmap
    font->texture = texture_png(filename);
    if(!font->texture.texture) return 0;
    
    printf("loaded %s\n", filename);
    return 1;
}

font_t* font_open(char* name) {
    // create a string large enough to fit the file extension
    int name_len = strlen(name);
    char* filename = malloc(name_len + 5);
    for(int i = 0; i < name_len; i++) {
        filename[i] = name[i];
    }
    filename[name_len]     = '.';
    filename[name_len + 4] = 0;
    char* extension = filename + name_len + 1;
    
    
    font_t* font = malloc(sizeof(font_t));
    font->name = name;
    
    int succeeded = font_load_fnt(font, filename, extension) &&
            font_load_img(font, filename, extension);
    
    free(filename);
    
    if(!succeeded) {
        free(font->chars);
        free(font);
        font = NULL;
    }
    
    return font;
}