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
    // background color
    background_color.h++;
    SDL_Color tmp = HsvToRgb(background_color);
    SDL_SetRenderDrawColor(RENDER.renderer, tmp.r, tmp.g, tmp.b, 255);
    // clear the screen
    SDL_RenderClear(RENDER.renderer);
    // draw some text

    draw_text(10, 10, -1, -1, "Menu Screen", (SDL_Color) {0, 255, 0});

    // draw buttons
    list_item* button = list_get_first_item(menu_data.buttons);
    while (button != NULL) {
        button_draw(button->data);
        button = button->next;
    }

    // draw to the screen
    SDL_RenderPresent(RENDER.renderer);

    Uint32 sleep_time = GLOBALS.sleep_time - (SDL_GetTicks() - GLOBALS.last_tick);

    if (sleep_time <= GLOBALS.sleep_time) {
        SDL_Delay(sleep_time);
        // printf("%llu\n", sleep_time);
    } else {
        printf("LAG!\n");
    }

    GLOBALS.last_tick = SDL_GetTicks();
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
