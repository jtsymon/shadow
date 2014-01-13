
#include <SDL2/SDL.h>

#include "button.h"


const int default_width = 150;
const int default_height = 50;
const SDL_Color default_foreground_color = { 255, 0, 0, 255 };
const SDL_Color default_background_color = { 200, 200, 200, 255 };
const SDL_Color default_hover_background_color = { 150, 150, 150, 255 };
const SDL_Color default_click_background_color = { 100, 100, 100, 255 };

button_t* button_create(char* text, int x, int y, void (*onclick)()) {
    return button_create__(text, x, y, onclick, default_width, default_height,
            default_foreground_color, default_background_color, default_hover_background_color, default_click_background_color);
}

button_t* button_create_(char* text, int x, int y, void (*onclick)(), int width, int height) {
    return button_create__(text, x, y, onclick, width, height,
            default_foreground_color, default_background_color, default_hover_background_color, default_click_background_color);
}

button_t* button_create__(char* text, int x, int y, void (*onclick)(), int width, int height,
        SDL_Color foreground_color, SDL_Color background_color, SDL_Color hover_background_color, SDL_Color click_background_color) {
    button_t* button = malloc(sizeof(button_t));
    button->text = text;
    button->x = x;
    button->y = y;
    button->onclick = onclick;
    button->width = width;
    button->height = height;
    button->foreground_color = foreground_color;
    button->background_color = background_color;
    button->hover_background_color = hover_background_color;
    button->click_background_color = click_background_color;
    return button;
}

bool button_hover(button_t* this, int x, int y) {
    return this->x < x && this->x + this->width > x && this->y < y && this->y + this->height > y;
}

void button_draw(button_t* this) {
    // draw background
    if(mouse.buttons[SDL_BUTTON_LEFT] && button_hover(this, mouse.click_start.x, mouse.click_start.y)) {
        glColor4ub(this->click_background_color.r, this->click_background_color.g, this->click_background_color.b, this->click_background_color.a);
    } else if(button_hover(this, mouse.x, mouse.y)) {
        glColor4ub(this->hover_background_color.r, this->hover_background_color.g, this->hover_background_color.b, this->hover_background_color.a);
    } else {
        glColor4ub(this->background_color.r, this->background_color.g, this->background_color.b, this->background_color.a);
    }
    fill_rectangle(this->x, this->y, this->x + this->width, this->y + this->height);
    
    // draw text
    glColor4ub(this->foreground_color.r, this->foreground_color.g, this->foreground_color.b, this->foreground_color.a);
    draw_text(this->x, this->y, this->text);
}

void button_click(button_t* this) {
    if(button_hover(this, mouse.click_start.x, mouse.click_start.y) && 
            button_hover(this, mouse.x, mouse.y) &&
            this->onclick != NULL) {
        this->onclick();
    }
}