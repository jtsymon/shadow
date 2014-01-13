
#include "graphics.h"

GLuint base; // Base Display List For The Font
GLuint texture; // Storage For Our Font Texture

/*
 * function to load in bitmap as a GL texture
 */
int LoadGLTextures() {
    // Create storage space for the texture
    SDL_Surface *TextureImage;

    // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
    if (TextureImage = SDL_LoadBMP("data/font.bmp")) {
        // Create The Texture
        glGenTextures(1, &texture);

        // Load in font
        // Typical Texture Generation Using Data From The Bitmap
        glBindTexture(GL_TEXTURE_2D, texture);

        // Generate The Texture
        glTexImage2D(GL_TEXTURE_2D, 0, 4, TextureImage->w, TextureImage->h,
                0, GL_BGRA, GL_UNSIGNED_BYTE, TextureImage->pixels);

        // Nearest Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        if (TextureImage) {
            SDL_FreeSurface(TextureImage);
        }
        return false;
    }
    if (TextureImage) {
        SDL_FreeSurface(TextureImage);
    }
    return true;
}

/*
 * function to build our font list
 */
GLvoid BuildFont() {
    float cx; // Holds Our X Character Coord
    float cy; // Holds Our Y Character Coord

    // Creating 256 Display List
    base = glGenLists(256);
    // Select Our Font Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Loop Through All 256 Lists
    for (int i = 0; i < 256; i++) {
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

        // X Position Of Current Character
        cx = 1 - (float) (i % 16) / 16.0f;
        // Y Position Of Current Character
        cy = 1 - (float) (i / 16) / 16.0f;

        // Start Building A List
        glNewList(base + (255 - i), GL_COMPILE);
        // Use A Quad For Each Character
        glBegin(GL_QUADS);
        // Texture Coord (Bottom Left)
        glTexCoord2f(cx - 0.0625, cy);
        // Vertex Coord (Bottom Left)
        glVertex2i(0, 0);

        // Texture Coord (Bottom Right)
        glTexCoord2f(cx, cy);
        // Vertex Coord (Bottom Right)
        glVertex2i(16, 0);

        // Texture Coord (Top Right)
        glTexCoord2f(cx, cy - 0.0625f);
        // Vertex Coord (Top Right)
        glVertex2i(16, 16);

        // Texture Coord (Top Left)
        glTexCoord2f(cx - 0.0625f, cy - 0.0625f);
        // Vertex Coord (Top Left)
        glVertex2i(0, 16);
        glEnd();

        // Move To The Left Of The Character
        glTranslated(10, 0, 0);
        glEndList();
    }
}

bool initFont() {
    // Load in the texture
    if (!LoadGLTextures()) {
        return false;
    }

    // Build our font list
    BuildFont();
    
    return true;
}

/*
 * Function to print the string
 */
void glPrint(int x, int y, char *string, int set) {
    
    if ( set > 1 )
	set = 1;

    /* Select our texture */
    glBindTexture(GL_TEXTURE_2D, texture);
    
    /* Store The Projection Matrix */
    glPushMatrix();
    /* Reset The Projection Matrix */
    glLoadIdentity();
    /* Set Up An Ortho Screen */
    glOrtho( 0, RENDER.width, 0, RENDER.height, -1, 1 );

    glTranslated(x, RENDER.height - y - 16, 0);

    /* Choose The Font Set (0 or 1) */
    glListBase(base - 32 + (128 * set));
    
    /* Write The Text To The Screen */
    glCallLists(strlen(string), GL_BYTE, string);
    
    /* Restore The Old Projection Matrix */
    glPopMatrix();
}