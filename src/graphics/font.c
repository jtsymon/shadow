
#include "font.h"
#include "texture.h"

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
    font->min_char = data[p++];
    font->max_char = data[p++];
    font->tex_size = pow(2, data[p++]);
    font->h        = data[p++];
    int num_chars = font->max_char - font->min_char;
    font->chars = malloc(num_chars * sizeof(font_char));
    for(int i = 0; i < num_chars; i++) {
        font->chars[i].x = data[p++];
        font->chars[i].y = data[p++];
        font->chars[i].w = data[p++];
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

int font_build(font_t* font) {
    float cx; // Holds x coord of current character
    float cy; // Holds y coord of current character
    float cw; // Holds width of current character
    float ch; // Holds height of current character
    
    ch = (float) (font->h) / font->tex_size;

    int size = font->max_char - font->min_char;
    
    // Creating Display List
    font->base = glGenLists(size);
    // Select Our Font Texture
    glBindTexture(GL_TEXTURE_2D, font->texture.texture);

    // Loop Through All Lists
    for (int i = 0; i < size; i++) {
        cx = (float) (font->chars[i].x) / font->tex_size;
        cy = (float) (font->chars[i].y) / font->tex_size;
        cw = (float) (font->chars[i].w) / font->tex_size;
        
        // Start Building A List
        glNewList(font->base + i, GL_COMPILE);
        // Use A Quad For Each Character
        glBegin(GL_QUADS);
        // Texture Coord (Bottom Left)
        glTexCoord2f(cx, 1 - cy - ch);
        // Vertex Coord (Bottom Left)
        glVertex2i(0, 0);

        // Texture Coord (Bottom Right)
        glTexCoord2f(cx + cw, 1 - cy - ch);
        // Vertex Coord (Bottom Right)
        glVertex2i(font->chars[i].w, 0);

        // Texture Coord (Top Right)
        glTexCoord2f(cx + cw, 1 - cy);
        // Vertex Coord (Top Right)
        glVertex2i(font->chars[i].w, font->h);

        // Texture Coord (Top Left)
        glTexCoord2f(cx, 1 - cy);
        // Vertex Coord (Top Left)
        glVertex2i(0, font->h);
        glEnd();

        // Move To The Left Of The Character
        glTranslated(font->chars[i].w, 0, 0);
        glEndList();
    }
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
            font_load_img(font, filename, extension) &&
            font_build(font);
    
    
    free(filename);
    
    if(!succeeded) {
        free(font->chars);
        free(font);
        font = NULL;
    }
    
    return font;
}