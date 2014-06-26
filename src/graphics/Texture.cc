#include "Texture.h"
#include <png.h>
#include "Graphics.h"
#include "../main.h"
#include "../misc.h"
#include <string.h>

static GLubyte *load_png(const char *name, unsigned int *width, unsigned int *height, int *bit_depth, int *colour_type) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int interlace_type;
    FILE *fp;

    if ((fp = fopen(name, "rb")) == NULL)
        return NULL;

    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);

    if (png_ptr == NULL) {
        fclose(fp);
        return NULL;
    }

    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }

    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return NULL;
    }

    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_get_IHDR(png_ptr, info_ptr, width, height, bit_depth, colour_type,
            &interlace_type, NULL, NULL);

    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    GLubyte *outData = (unsigned char*) malloc(row_bytes * *height);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (int i = 0; i < *width; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(outData + (row_bytes * (*height - 1 - i)), row_pointers[i], row_bytes);
    }

    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Close the file */
    fclose(fp);

    /* That's it */
    return outData;
}

Texture::Texture(const std::string &filename) : texture(0) {
    GLubyte *image_data = load_png(filename.c_str(), &this->width, &this->height, &this->bit_depth, &this->colour_type);
    if (!image_data) {
        throw Exception("Failed to load texture");
    }

    // Create The Texture
    glGenTextures(1, &this->texture);

    // Load in font
    // Typical texture generation using data from the bitmap
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
            0, GL_BGRA, GL_UNSIGNED_BYTE, image_data);

    // Nearest filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    free(image_data);
}

static void _draw_texture(GLuint texture, RGBA colour, const GLfloat data[]) {

    GLuint shader = Graphics::shaders[GRAPHICS_TEXTURE_SHADER];
    GLuint texID = glGetUniformLocation(shader, "texture");
    glUseProgram(shader);
    // Bind our texture in Texture Unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);

    glUniform4f(glGetUniformLocation(shader, "colour_in"),
            (float) colour.r / 255, (float) colour.g / 255,
            (float) colour.b / 255, (float) colour.a / 255);

    glBindVertexArray(Graphics::vertex_array[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof (GLfloat), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
    glVertexAttribPointer(
            0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
            4, // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*) 0 // array buffer offset
            );
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableVertexAttribArray(0);
}

void Texture::draw(RGBA colour, int x, int y, int w, int h) {

    const GLfloat points[] = {
        game_to_gl_x(x), game_to_gl_y(y), 0, 0,
        game_to_gl_x(x + w), game_to_gl_y(y), 1, 0,
        game_to_gl_x(x + w), game_to_gl_y(y + h), 1, 1,
        game_to_gl_x(x), game_to_gl_y(y + h), 0, 1
    };

    _draw_texture(this->texture, colour, points);
}

void Texture::drawRegion(RGBA colour, int x, int y, int w, int h, int tx, int ty, int tw, int th) {

    const GLfloat points[] = {
        game_to_gl_x(x), game_to_gl_y(y), (float) (tx) / this->width, (float) (ty) / this->height,
        game_to_gl_x(x + w), game_to_gl_y(y), (float) (tx + tw) / this->width, (float) (ty) / this->height,
        game_to_gl_x(x + w), game_to_gl_y(y + h), (float) (tx + tw) / this->width, (float) (ty + th) / this->height,
        game_to_gl_x(x), game_to_gl_y(y + h), (float) (tx) / this->width, (float) (ty + th) / this->height
    };

    _draw_texture(this->texture, colour, points);
}