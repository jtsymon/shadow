
#include "graphics.h"

void update_dimensions() {
    two_over_width  = 2.0 / RENDER.width;
    two_over_height = 2.0 / RENDER.height;
    
    printf("2/w=%f, 2/h=%f\n", two_over_width, two_over_height);
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
    for(i = 0; i < n_buffers; i++) {
        glGenVertexArrays(1, &RENDER.vertex_array[i]);
        glBindVertexArray(RENDER.vertex_array[i]);

        glGenBuffers(1, &RENDER.vertex_buffer[i]);
    }
    
    return 0;
}

int init_shaders() {
    RENDER.shaders.shadow  = create_program_src("shaders/pass_through_texture.vert", "shaders/shadow_mask.frag");
    RENDER.shaders.colour  = create_program_src("shaders/pass_through.vert", "shaders/colour.frag");
    RENDER.shaders.texture = create_program_src("shaders/texture.vert", "shaders/texture.frag");
    
    return !(RENDER.shaders.shadow || RENDER.shaders.colour || RENDER.shaders.texture);
}

int init_mask() {
    // The framebuffer, which regroups 0 or more textures, and 0 or 1 depth buffer.
    glGenFramebuffers(1, &RENDER.mask.framebuffer);
    // The texture to render to
    glGenTextures(1, &RENDER.mask.texture);
    
    glBindFramebuffer(GL_FRAMEBUFFER, RENDER.mask.framebuffer);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, RENDER.mask.texture);

    // Give an empty image to OpenGL ( the last "0" means "empty" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RENDER.width, RENDER.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, RENDER.mask.texture, 0);

    //    // Set the list of draw buffers.
    //    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    //    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fputs("Failed to generate framebuffers", stderr);
        return 2;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
//    static const GLfloat fullscreen_quad[] = {
//        -1.0f, -1.0f, 0.0f,
//        1.0f, -1.0f, 0.0f,
//        -1.0f, 1.0f, 0.0f,
//        -1.0f, 1.0f, 0.0f,
//        1.0f, -1.0f, 0.0f,
//        1.0f, 1.0f, 0.0f,
//    };

    glGenVertexArrays(1, &RENDER.mask.vertex_array);
	glBindVertexArray(RENDER.mask.vertex_array);
    
    glGenBuffers(1, &RENDER.mask.vertex_buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, RENDER.mask.vertex_buffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof (fullscreen_quad), fullscreen_quad, GL_STATIC_DRAW);
    
    return 0;
}

/**
 *  General OpenGL initialization function
 */
int initGL() {

    if(!init_fonts()) {
        printf("Failed to load font\n");
        return 1;
    }
    
    glViewport(0,0, RENDER.width, RENDER.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Enable smooth shading
    glShadeModel( GL_SMOOTH );
    
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
    
    return init_shaders() + init_mask() + init_vertex_buffers() + init_buffer();
}

double game_to_gl_x(int x) {
    return (double) x * two_over_width - 1;
}

double game_to_gl_y(int y) {
    return 1 - (double) y * two_over_height;
}

void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    buffer_set_mode(GL_TRIANGLES);
    buffer_add_n(3, (GLfloat[]) {
        game_to_gl_x(x1), game_to_gl_y(y1),
        game_to_gl_x(x2), game_to_gl_y(y2),
        game_to_gl_x(x3), game_to_gl_y(y3)
    });
}

void fill_rectangle(int x1, int y1, int x2, int y2) {
    double xa = game_to_gl_x(x1), ya = game_to_gl_y(y1),
           xb = game_to_gl_x(x2), yb = game_to_gl_y(y2);
    
    buffer_set_mode(GL_TRIANGLES);
    buffer_add_n(6, (GLfloat[]) {
        xa, ya,
        xb, ya,
        xb, yb,
        xa, ya,
        xa, yb,
        xb, yb
    });
}