
#include "font.h"

bool font_load_fnt(font_t* font, char* filename, char* extension) {
    extension[0] = 'f';
    extension[1] = 'n';
    extension[2] = 't';
    
    printf("filename: %s\n", filename);
    
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
    int num_chars = font->max_char - font->min_char;
    font->chars = malloc(num_chars * sizeof(font_char));
    for(int i = 0; i < num_chars; i++) {
        font->chars[i].x = data[p++];
        font->chars[i].y = data[p++];
        font->chars[i].w = data[p++];
        font->chars[i].h = data[p++];
    }
    free(data);
    
    printf("loaded .fnt\n");
    return true;
}

bool font_load_bmp(font_t* font, char* filename, char* extension) {
    extension[0] = 'b';
    extension[1] = 'm';
    extension[2] = 'p';

    printf("filename: %s\n", filename);
    
    // Load font bitmap
    SDL_Surface *font_bitmap = SDL_LoadBMP(filename);
    if(!font_bitmap) {
        return false;
    }
    // Create The Texture
    glGenTextures(1, &(font->texture));

    // Load in font
    // Typical texture generation using data from the bitmap
    glBindTexture(GL_TEXTURE_2D, font->texture);

    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, 4, font_bitmap->w, font_bitmap->h,
            0, GL_BGRA, GL_UNSIGNED_BYTE, font_bitmap->pixels);

    // Nearest filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(font_bitmap);
    
    printf("loaded .bmp\n");
    return true;
}

bool font_build(font_t* font) {
    float cx; // Holds x coord of current character
    float cy; // Holds y coord of current character
    float cw; // Holds width of current character
    float ch; // Holds height of current character

    int size = font->max_char - font->min_char;
    
    // Creating Display List
    font->base = glGenLists(size);
    // Select Our Font Texture
    glBindTexture(GL_TEXTURE_2D, font->texture);
    
    printf("tex_size: %d\n", font->tex_size);

    // Loop Through All Lists
    for (int i = 0; i < size; i++) {
        /* NOTE:
         *  BMPs are stored with the top-leftmost pixel being the
         * last byte and the bottom-rightmost pixel being the first
         * byte. So an image that is displayed as
         *    1 0
         *    0 0
         * is represented data-wise like
         *    0 0
         *    0 1
         * And because SDL_LoadBMP loads the raw data without
         * translating to how it is thought of when viewed we need
         * to start at the bottom-right corner of the data and work
         * backwards to get everything properly. So the below code has
         * been modified to reflect this. Examine how this is done and
         * how the original tutorial is done to grasp the differences.
         *
         * As a side note BMPs are also stored as BGR instead of RGB
         * and that is why we load the texture using GL_BGR. It's
         * bass-ackwards I know but whattaya gonna do?
         */

        cx = 1 - (float) (font->chars[i].x) / font->tex_size;
        cy = 1 - (float) (font->chars[i].y) / font->tex_size;
        cw = (float) (font->chars[i].w) / font->tex_size * 10;
        ch = (float) (font->chars[i].h) / font->tex_size * 10;
        
        printf("%f,%f %f,%f\n", cx, cy, cw, ch);

        // Start Building A List
        glNewList(font->base + (size - i), GL_COMPILE);
        // Use A Quad For Each Character
        glBegin(GL_QUADS);
        // Texture Coord (Bottom Left)
        glTexCoord2f(cx - cw, cy);
        // Vertex Coord (Bottom Left)
        glVertex2i(0, 0);

        // Texture Coord (Bottom Right)
        glTexCoord2f(cx, cy);
        // Vertex Coord (Bottom Right)
        glVertex2i(font->chars[i].w, 0);

        // Texture Coord (Top Right)
        glTexCoord2f(cx, cy - ch);
        // Vertex Coord (Top Right)
        glVertex2i(font->chars[i].w, font->chars[i].h);

        // Texture Coord (Top Left)
        glTexCoord2f(cx - cw, cy - ch);
        // Vertex Coord (Top Left)
        glVertex2i(0, font->chars[i].h);
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
    
    bool succeeded = font_load_fnt(font, filename, extension) &&
            font_load_bmp(font, filename, extension) &&
            font_build(font);
    
    
    free(filename);
    
    if(!succeeded) {
        free(font->chars);
        free(font);
        font = NULL;
    }
    
    return font;
}