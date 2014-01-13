
// #include <SDL2/SDL_rect.h>

#include "opengl.h"

double two_over_width, two_over_height;

void update_dimensions() {
    two_over_width  = 2.0 / RENDER.width;
    two_over_height = 2.0 / RENDER.height;
    
    printf("2/w=%f, 2/h=%f\n", two_over_width, two_over_height);
}

/**
 *  General OpenGL initialization function
 */
int initGL() {
    
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    // Create the window where we will draw.
    if((RENDER.window = SDL_CreateWindow("shadow",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            RENDER.width, RENDER.height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)) == NULL) {
        printf("%s\n", SDL_GetError());
        return 2;
    }
    
    if((RENDER.gl_context = SDL_GL_CreateContext(RENDER.window)) == NULL) {
        printf("%s\n", SDL_GetError());
        return 3;
    }
    
    SDL_GL_MakeCurrent(RENDER.window, RENDER.gl_context); 
    SDL_GL_SetSwapInterval(1);
    
    glViewport(0,0, RENDER.width, RENDER.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // 2d
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    update_dimensions();
    return 0;
}

double game_to_gl_x(int x) {
    return (double) x * two_over_width - 1;
}

double game_to_gl_y(int y) {
    return 1 - (double) y * two_over_height;
}

void set_color_ab(Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool blending) {
    if(blending) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
    glColor4ub(r, g, b, a);
}

void set_color_a(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    set_color_ab(r, g, b, a, true);
}

void set_color(Uint8 r, Uint8 g, Uint8 b) {
    set_color_ab(r, g, b, 255, true);
}

void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    glBegin(GL_TRIANGLES);

    glVertex2d(game_to_gl_x(x1), game_to_gl_y(y1));
    glVertex2d(game_to_gl_x(x2), game_to_gl_y(y2));
    glVertex2d(game_to_gl_x(x3), game_to_gl_y(y3));

    glEnd();
}

void fill_rectangle(int x1, int y1, int x2, int y2) {
    glRectd(game_to_gl_x(x1), game_to_gl_y(y1), game_to_gl_x(x2), game_to_gl_y(y2));
}

void draw_text(int x, int y, char* text) {
    printf("text drawing not yet implemented [%s]\n", text);
}
