#include "menu.h"

HsvColor background_color;

void play_game() {
    set_screen(GLOBALS.screen_game);
}

void screen_menu_create() {
    menu_data.buttons = list_init();
    list_add(menu_data.buttons, button_create("Play", 100, 100, play_game));
    list_add(menu_data.buttons, button_create("Create map", 100, 160, NULL));
    background_color = (HsvColor) { 0, 100, 100 };
    printf("created menu\n");
}

void screen_menu_destroy() {
    printf("Cleaning up menu screen...\n");
    list_free_all(menu_data.buttons);
}

void screen_menu_show() {

}

void screen_menu_hide() {

}

void screen_menu_render() {
    
    while (SDL_PollEvent(&RENDER.e)) {
        switch (RENDER.e.type) {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse.buttons[RENDER.e.button.button] = true;
                if (RENDER.e.button.button == SDL_BUTTON_LEFT) {
                    mouse.click_start.x = mouse.x;
                    mouse.click_start.y = mouse.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                mouse.buttons[RENDER.e.button.button] = false;
                if (RENDER.e.button.button == SDL_BUTTON_LEFT) {
                    list_item* button = list_get_first_item(menu_data.buttons);
                    while (button != NULL) {
                        button_click((button_t*) (button->data));
                        button = button->next;
                    }
                }
                break;
            case SDL_KEYUP:
                switch (RENDER.e.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        running = SDL_FALSE;
                        break;
                }
                break;
        }
    }
    keymod = SDL_GetModState();
    if (keys[SDL_SCANCODE_A]) {
        printf("DEBUG ");
        SDL_Delay(10);
    }
    if (keys[SDL_SCANCODE_S]) {
        printf("DEBUG ");
        SDL_Delay(5);
    }
    
    /*SDL_SetRenderDrawColor(RENDER.renderer, tmp.r, tmp.g, tmp.b, 255);
    // clear the screen
    SDL_RenderClear(RENDER.renderer);
    // draw some text

    draw_text(10, 10, -1, -1, "Menu Screen", (SDL_Color) {0, 255, 0});

    */
    
    // background color
    background_color.h++;
    SDL_Color tmp = HsvToRgb(background_color);
    glClearColor((float)tmp.r / 255, (float)tmp.g / 255, (float)tmp.b / 255, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // glColor4ub(255, 0, 0, 255);
    // fill_rectangle(320, 0, 640, 640);
    // set_color_a(0, 255, 0, 128);
    // fill_triangle(0, 0, 640, 0, 640, 640);
    
    // draw buttons
    list_item* button = list_get_first_item(menu_data.buttons);
    while (button != NULL) {
        button_draw(button->data);
        button = button->next;
    }
    
    glColor3ub(0, 255, 0);
    draw_text(100, 300, "Testing Text _ <- underscore");
    
    draw_texture(100, 400, 256, 256);
    
    SDL_GL_SwapWindow(RENDER.window);
}

void (*screen_menu_vtable[])() = {&screen_menu_create,
    &screen_menu_destroy,
    &screen_menu_show,
    &screen_menu_hide,
    &screen_menu_render};

screen_t* screen_menu() {
    screen_t* screen = malloc(sizeof (screen_t));
    screen->f = screen_menu_vtable;
    screen_menu_create();
    return screen;
}
