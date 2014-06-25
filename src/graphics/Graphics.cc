
#include "Graphics.h"
#include "../misc.h"
#include "Colours.h"
#include "BitmapFont.h"
#include "Shaders.h"
#include "../main.h"

GLuint Graphics::vertex_buffer[n_buffers];
GLuint Graphics::vertex_array[n_buffers];
GLuint Graphics::shaders[n_shaders];

Graphics::Graphics() : colour(RGBA(0, 0, 0, 255)) {
    // init shaders
    Graphics::shaders[GRAPHICS_COLOUR_SHADER] = compile_program("shaders/pass_through.vert", "shaders/colour.frag");
    Graphics::shaders[GRAPHICS_SHADOW_SHADER] = compile_program("shaders/pass_through_texture.vert", "shaders/shadow_mask.frag");
    Graphics::shaders[GRAPHICS_TEXTURE_SHADER] = compile_program("shaders/texture.vert", "shaders/texture.frag");
}

Graphics& Graphics::get() {
    static Graphics instance;
    return instance;
}

static float two_over_width, two_over_height;

void update_dimensions() {
    two_over_width = 2.f / width;
    two_over_height = 2.f / height;

    printf("2/w=%f, 2/h=%f\n", two_over_width, two_over_height);
}

float game_to_gl_x(int x) {
    return (float) x * two_over_width - 1;
}

float game_to_gl_y(int y) {
    return 1 - (float) y * two_over_height;
}

/* void getTexture() {
    SDL_Surface* surface = SDL_LoadBMP("data/font.bmp");
    if(surface) {
        
    // Check that the image's width is a power of 2
    if ( (surface->w & (surface->w - 1)) != 0 ) {
        printf("warning: image.bmp's width is not a power of 2\n");
    }
 
    // Also check if the height is a power of 2
    if ( (surface->h & (surface->h - 1)) != 0 ) {
        printf("warning: image.bmp's height is not a power of 2\n");
    }
 
        // get the number of channels in the SDL surface
        int texture_format, nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        } else {
                printf("warning: the image is not truecolor..  this will probably break\n");
                // this error should not go unhandled
        }
        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );
        
        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );

        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    }
} */

int init_vertex_buffers() {
    int i;
    for (i = 0; i < n_buffers; i++) {
        glGenVertexArrays(1, &Graphics::vertex_array[i]);
        glBindVertexArray(Graphics::vertex_array[i]);

        glGenBuffers(1, &Graphics::vertex_buffer[i]);
    }

    return 0;
}

int init_mask() {
    //        // The framebuffer, which regroups 0 or more textures, and 0 or 1 depth buffer.
    //        glGenFramebuffers(1, &RENDER.mask.framebuffer);
    //        // The texture to render to
    //        glGenTextures(1, &RENDER.mask.texture);
    //
    //        glBindFramebuffer(GL_FRAMEBUFFER, RENDER.mask.framebuffer);
    //
    //        // "Bind" the newly created texture : all future texture functions will modify this texture
    //        glBindTexture(GL_TEXTURE_2D, RENDER.mask.texture);
    //
    //        // Give an empty image to OpenGL ( the last "0" means "empty" )
    //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RENDER.width, RENDER.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    //
    //        // Poor filtering
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //
    //        // Set "renderedTexture" as our colour attachement #0
    //        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, RENDER.mask.texture, 0);
    //
    //        //    // Set the list of draw buffers.
    //        //    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    //        //    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    //
    //        // Always check that our framebuffer is ok
    //        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    //            fputs("Failed to generate framebuffers", stderr);
    //            return 2;
    //        }
    //        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //
    //        //    static const GLfloat fullscreen_quad[] = {
    //        //        -1.0f, -1.0f, 0.0f,
    //        //        1.0f, -1.0f, 0.0f,
    //        //        -1.0f, 1.0f, 0.0f,
    //        //        -1.0f, 1.0f, 0.0f,
    //        //        1.0f, -1.0f, 0.0f,
    //        //        1.0f, 1.0f, 0.0f,
    //        //    };
    //
    //        glGenVertexArrays(1, &RENDER.mask.vertex_array);
    //        glBindVertexArray(RENDER.mask.vertex_array);
    //
    //        glGenBuffers(1, &RENDER.mask.vertex_buffer);
    //        // glBindBuffer(GL_ARRAY_BUFFER, RENDER.mask.vertex_buffer);
    //        // glBufferData(GL_ARRAY_BUFFER, sizeof (fullscreen_quad), fullscreen_quad, GL_STATIC_DRAW);

    return 0;
}

/**
 *  General OpenGL initialization function
 */
int initGL() {

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Enable smooth shading
    glShadeModel(GL_SMOOTH);

    //    glEnable( GL_LINE_SMOOTH );
    //    glEnable( GL_POLYGON_SMOOTH );
    //    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    //    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    // 2d
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    update_dimensions();

    return init_mask() + init_vertex_buffers();
}

void Graphics::set_mode(GLenum mode) {
    if (mode == this->mode) return;
    this->flush();
    this->mode = mode;
}

void Graphics::set_colour(RGBA colour) {
    if (colour.r == this->colour.r && colour.g == this->colour.g &&
            colour.b == this->colour.b && colour.a == this->colour.a) return;
    this->flush();
    this->colour = colour;
}

void Graphics::set_shader(int shader=0) {
    if (shader == this->shader) return;
    this->flush();
    this->shader = shader;
}

void Graphics::flush() {
    if (this->p == 0) return;

    GLuint shader = this->shaders[this->shader];
    
    glUseProgram(shader);

    glBindVertexArray(Graphics::vertex_array[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, this->p * 2 * sizeof (GLfloat), this->buffer, GL_STATIC_DRAW);

    glUniform4f(glGetUniformLocation(shader, "colour_in"),
            (float) this->colour.r / 255, (float) this->colour.g / 255,
            (float) this->colour.b / 255, (float) this->colour.a / 255);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Graphics::vertex_buffer[0]);
    glVertexAttribPointer(
            0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
            2, // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*) 0 // array buffer offset
            );
    glDrawArrays(this->mode, 0, this->p);
    glDisableVertexAttribArray(0);

    this->reset();
}

void Graphics::reset() {
    this->p = 0;
}

int Graphics::check_space() {
    return render_buffer_size - this->p;
}

int Graphics::add(GLfloat x, GLfloat y) {
    if (render_buffer_size - this->p <= 0) {
        this->flush();
    }
    this->buffer[this->p << 1] = x;
    this->buffer[(this->p << 1) + 1] = y;
    return render_buffer_size - ++this->p;
}

int Graphics::add(int n, const GLfloat points[]) {
    if (render_buffer_size - this->p <= n - 1) {
        this->flush();
    }
    int i;
    for (i = 0; i < n; i++) {
        this->buffer[this->p << 1] = points[i << 1];
        this->buffer[(p << 1) + 1] = points[(i << 1) + 1];
        this->p++;
    }
    return render_buffer_size - this->p;
}

void Graphics::fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    this->set_mode(GL_TRIANGLES);

    const GLfloat points[] = {
        game_to_gl_x(x1), game_to_gl_y(y1),
        game_to_gl_x(x2), game_to_gl_y(y2),
        game_to_gl_x(x3), game_to_gl_y(y3)
    };

    this->add(3, points);
}

void Graphics::fill_rectangle(int x1, int y1, int x2, int y2) {
    float xa = game_to_gl_x(x1), ya = game_to_gl_y(y1),
            xb = game_to_gl_x(x2), yb = game_to_gl_y(y2);

    this->set_mode(GL_TRIANGLES);

    const GLfloat points[] = {
        xa, ya,
        xb, ya,
        xb, yb,
        xa, ya,
        xa, yb,
        xb, yb
    };

    this->add(6, points);
}
