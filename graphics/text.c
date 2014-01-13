
#include "text.h"

bool initFont() {
    font = font_open("data/default");
    return (font != NULL);
}

/*
 * Function to print the string
 */
void glPrint(int x, int y, char *string) {

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
    glListBase(font->base);
    
    /* Write The Text To The Screen */
    glCallLists(strlen(string), GL_BYTE, string);
    
    /* Restore The Old Projection Matrix */
    glPopMatrix();
}