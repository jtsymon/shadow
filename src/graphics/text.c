
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
    
    GLfloat coords[len * 4 * 6];
    
    // Generate an array of coords to draw the font texture
    for(i = 0; i < len; i++) {
        int c = text[i] - font->min_char;
        if(c < 0 || text[i] > font->max_char) {
            fprintf(stderr, "Cannot print character '%s'\n", text[i]);
        }
        
        // right edge of character
        x += font->w[c];
        double cx2 = game_to_gl_x(x);
        
        int tc = 4 * c;
        GLfloat tx1 = font->chars[tc++];
        GLfloat ty1 = font->chars[tc++];
        GLfloat tx2 = font->chars[tc++];
        GLfloat ty2 = font->chars[tc++];
        
        int d = i * 4 * 6;
        
        // top left
        coords[d++] = cx1;
        coords[d++] = cy1;
        coords[d++] = tx1;
        coords[d++] = ty1;
        // top right
        coords[d++] = cx2;
        coords[d++] = cy1;
        coords[d++] = tx2;
        coords[d++] = ty1;
        // bottom right
        coords[d++] = cx2;
        coords[d++] = cy2;
        coords[d++] = tx2;
        coords[d++] = ty2;
        // top left
        coords[d++] = cx1;
        coords[d++] = cy1;
        coords[d++] = tx1;
        coords[d++] = ty1;
        // bottom left
        coords[d++] = cx1;
        coords[d++] = cy2;
        coords[d++] = tx1;
        coords[d++] = ty2;
        // bottom right
        coords[d++] = cx2;
        coords[d++] = cy2;
        coords[d++] = tx2;
        coords[d++] = ty2;
        
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
    
    glBindVertexArray(RENDER.vertex_array[0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, RENDER.vertex_buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, len * 4 * 6 * sizeof(GLfloat), coords, GL_STATIC_DRAW);
    glVertexAttribPointer(
                    0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    4, // size
                    GL_FLOAT, // type
                    GL_FALSE, // normalized?
                    0, // stride
                    (void*) 0 // array buffer offset
                    );
    
    glDrawArrays(GL_TRIANGLES, 0, len * 6);
    glDisableVertexAttribArray(0);
}

void draw_text(int x, int y, char* text, RGBA colour) {
    draw_text_f(x, y, text, colour, default_font);
}

int text_width(char* text, font_t* font) {
    int w = 0, p = 0;
    for(; text[p] != 0 ;) {
        w += font->w[text[p++] - font->min_char];
    }
    return w;
}