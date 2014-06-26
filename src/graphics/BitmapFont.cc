#include "BitmapFont.h"
#include "../misc.h"
#include "../math/math.h"
#include <fstream>
#include <iostream>

BitmapFont *BitmapFont::standard() {
    static BitmapFont *font = new BitmapFont("data/default");
    return font;
}

BitmapFont *BitmapFont::bold_oblique() {
    static BitmapFont *font = new BitmapFont("data/bold_oblique");
    return font;
}

BitmapFont::BitmapFont(const std::string &name) : name(name) {

    char *data;

    std::ifstream file((name + ".fnt").c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streampos size = file.tellg();
        data = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(data, size);
        file.close();

        int p = 0;
        this->min_char = data[p++];
        this->max_char = data[p++];
        this->tex_size = pow(2, data[p++]);
        this->h = data[p++];
        int num_chars = this->max_char - this->min_char;
        this->chars = (GLfloat*) malloc(num_chars * 4 * sizeof (GLfloat));
        this->w = (int*) malloc(num_chars * sizeof (int));

        int i;
        for (i = 0; i < num_chars; i++) {
            int x = data[p++];
            int y = data[p++];
            this->w[i] = data[p++];
            int c = i * 4;
            this->chars[c++] = (GLfloat) x / this->tex_size;
            this->chars[c++] = (GLfloat) y / this->tex_size;
            this->chars[c++] = (GLfloat) (x + this->w[i]) / this->tex_size;
            this->chars[c++] = (GLfloat) (y + this->h) / this->tex_size;
        }

        delete[] data;
    } else throw Exception("Failed to load font config");

    this->texture = new Texture(name + ".png");

    std::cout << "Loaded font '" << name << "'" << std::endl;
}

int BitmapFont::textWidth(const std::string &text) {
    int w = 0, p = 0;
    for (; text[p] != 0;) {
        w += this->w[text[p++] - this->min_char];
    }
    return w;
}

void BitmapFont::drawText(int x, int y, const std::string &text, RGBA colour) {

    Graphics g = Graphics::get();

    int i;
    int len = text.length();

    double cx1 = game_to_gl_x(x);
    double cy1 = game_to_gl_y(y);
    double cy2 = game_to_gl_y(y + this->h);

    GLfloat coords[len * 4 * 6];
    
    const char *str = text.c_str();

    // Generate an array of coords to draw the font texture
    for (i = 0; i < len; i++) {
        int c = str[i] - this->min_char;
        if (c < 0 || str[i] > this->max_char) {
            fprintf(stderr, "Cannot print character '%s'\n", str[i]);
        }

        // right edge of character
        x += this->w[c];
        double cx2 = game_to_gl_x(x);

        int tc = 4 * c;
        GLfloat tx1 = this->chars[tc++];
        GLfloat ty1 = this->chars[tc++];
        GLfloat tx2 = this->chars[tc++];
        GLfloat ty2 = this->chars[tc++];

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

    GLuint shader = Graphics::shaders[GRAPHICS_TEXTURE_SHADER];
    
    GLuint texID = glGetUniformLocation(shader, "texture");
    glUseProgram(shader);
    // Bind our texture in Texture Unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture->getTexture());
    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);

    glUniform4f(glGetUniformLocation(shader, "colour_in"),
            (float) colour.r / 255, (float) colour.g / 255,
            (float) colour.b / 255, (float) colour.a / 255);

    glBindVertexArray(g.vertex_array[0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, g.vertex_buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, len * 4 * 6 * sizeof (GLfloat), coords, GL_STATIC_DRAW);
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
