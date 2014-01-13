
#include "text.h"

bool initFonts() {
    font_default = font_open("data/default");
    font_bold_oblique = font_open("data/bold_oblique");
    return font_default && font_bold_oblique;
}

/*
 * Function to print the string
 */
void glPrint(int x, int y, char *string, font_t* font) {

    /* Select our texture */
    glBindTexture(GL_TEXTURE_2D, font->texture);
    
    /* Store The Projection Matrix */
    glPushMatrix();
    /* Reset The Projection Matrix */
    glLoadIdentity();
    /* Set Up An Ortho Screen */
    glOrtho( 0, RENDER.width, 0, RENDER.height, -1, 1 );

    glTranslated(x, RENDER.height - y - 16, 0);

    /* Choose The Font Set (0 or 1) */
    glListBase(font->base - font->min_char);
    
    /* Write The Text To The Screen */
    glCallLists(strlen(string), GL_BYTE, string);
    
    /* Restore The Old Projection Matrix */
    glPopMatrix();
}

int text_height_font(char* text, font_t* font) {
    return font->h;
}
int text_height(char* text) {
    return text_height_font(text, font_default);
}

int text_width_font(char* text, font_t* font) {
    int w = 0, p = 0;
    for(; text[p] != 0 ;) {
        w += font->chars[text[p++] - font->min_char].w;
    }
    return w;
}
int text_width(char* text) {
    return text_width_font(text, font_default);
}