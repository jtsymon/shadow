
#include "text.h"
#include "graphics.h"

int init_fonts() {
    default_font = font_open("data/default");
    font_bold_oblique = font_open("data/bold_oblique");
    return default_font && font_bold_oblique;
}

/*
 * Function to print the string
 */
void draw_text_f(int x, int y, char *text, RGBA colour, font_t* font) {
    
    int i;
    int len = strlen(text);
    
    double cx1 = game_to_gl_x(x);
    double cy1 = game_to_gl_y(y);
    double cy2 = game_to_gl_y(y + font->h);
    
    GLfloat data[len * 4 * 6];
    
    // Generate an array of coords to draw the font texture
    for(i = 0; i < len; i++) {
        int c = text[i] - font->min_char;
        if(c < 0 || text[i] > font->max_char) {
            fprintf(stderr, "Cannot print character '%s'\n", text[i]);
        }
        
        font_char ch = font->chars[c];
        
        // right edge of character
        x += ch.w;
        double cx2 = game_to_gl_x(x);
        
        double tx1 = (double)ch.x / font->tex_size;
        double ty1 = (double)ch.y / font->tex_size;
        double tx2 = (double)(ch.x + ch.w) / font->tex_size;
        double ty2 = (double)(ch.y + font->h) / font->tex_size;
        
        int d = i * 4 * 6;
        
        // top left
        data[d++] = cx1;
        data[d++] = cy1;
        data[d++] = tx1;
        data[d++] = ty1;
        // top right
        data[d++] = cx2;
        data[d++] = cy1;
        data[d++] = tx2;
        data[d++] = ty1;
        // bottom right
        data[d++] = cx2;
        data[d++] = cy2;
        data[d++] = tx2;
        data[d++] = ty2;
        // top left
        data[d++] = cx1;
        data[d++] = cy1;
        data[d++] = tx1;
        data[d++] = ty1;
        // bottom left
        data[d++] = cx1;
        data[d++] = cy2;
        data[d++] = tx1;
        data[d++] = ty2;
        // bottom right
        data[d++] = cx2;
        data[d++] = cy2;
        data[d++] = tx2;
        data[d++] = ty2;
        
        // shift across to the next character
        cx1 = cx2;
    }
    
    GLuint texID = glGetUniformLocation(RENDER.shaders.texture, "texture");
    glUseProgram(RENDER.shaders.texture);
    // Bind our texture in Texture Unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->texture.texture);
    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);
    
    glUniform4f(glGetUniformLocation(RENDER.shaders.texture, "colour_in"),
            (float)colour.r / 255, (float)colour.g / 255,
            (float)colour.b / 255, (float)colour.a / 255);
    
    glBindVertexArray(RENDER.vertex_array);
    
    glBindBuffer(GL_ARRAY_BUFFER, RENDER.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, len * 4 * 6 * sizeof(GLfloat), data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, RENDER.vertex_buffer);
    glVertexAttribPointer(
                    0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    4, // size
                    GL_FLOAT, // type
                    GL_FALSE, // normalized?
                    0, // stride
                    (void*) 0 // array buffer offset
                    );
    glDrawArrays(GL_TRIANGLES, 0, len * 4 * 6);
    glDisableVertexAttribArray(0);
//    /* Select our texture */
//    glBindTexture(GL_TEXTURE_2D, font->texture.texture);
//    
//    /* Store The Projection Matrix */
//    glPushMatrix();
//    /* Reset The Projection Matrix */
//    glLoadIdentity();
//    /* Set Up An Ortho Screen */
//    glOrtho( 0, RENDER.width, 0, RENDER.height, -1, 1 );
//
//    glTranslated(x, RENDER.height - y - 16, 0);
//
//    /* Choose The Font Set (0 or 1) */
//    glListBase(font->base - font->min_char);
//    
//    /* Write The Text To The Screen */
//    glCallLists(strlen(string), GL_BYTE, string);
//    
//    /* Restore The Old Projection Matrix */
//    glPopMatrix();
}

void draw_text(int x, int y, char* text, RGBA colour) {
    draw_text_f(x, y, text, colour, default_font);
}

int text_width(char* text, font_t* font) {
    int w = 0, p = 0;
    for(; text[p] != 0 ;) {
        w += font->chars[text[p++] - font->min_char].w;
    }
    return w;
}