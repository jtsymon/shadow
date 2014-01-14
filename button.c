
#include <SDL2/SDL.h>

#include "button.h"


const int default_width = 150;
const int default_height = 50;
const SDL_Color default_foreground_color = { 255, 0, 0, 255 };
const SDL_Color default_background_color = { 200, 200, 200, 255 };
const SDL_Color default_hover_background_color = { 150, 150, 150, 255 };
const SDL_Color default_click_background_color = { 100, 100, 100, 255 };

void button_update_text_position(button_t* this) {
    this->text_x = this->x + (this->w - text_width(this->text, this->font)) / 2;
    this->text_y = this->y + (this->h - (this->font->h / 2)) / 2;
}

void button_set_position(button_t* this, int x, int y) {
    this->x = x;
    this->y = y;
    button_update_text_position(this);
}

void button_set_size(button_t* this, int w, int h) {
    this->w = w;
    this->h = h;
    button_update_text_position(this);
}

void button_set_text(button_t* this, char* text) {
    this->text = text;
    button_update_text_position(this);
}

void button_set_font(button_t* this, font_t* font) {
    this->font = font;
    button_update_text_position(this);
}

button_t* button_create_size_color_font(char* text, int x, int y, void (*onclick)(), int width, int height,
        SDL_Color foreground_color, SDL_Color background_color, SDL_Color hover_background_color, SDL_Color click_background_color,
        font_t* font) {
    button_t* button = malloc(sizeof(button_t));
    button->x = x;
    button->y = y;
    button->text = text;
    button->onclick = onclick;
    button->w = width;
    button->h = height;
    button->foreground_color = foreground_color;
    button->background_color = background_color;
    button->hover_background_color = hover_background_color;
    button->click_background_color = click_background_color;
    button->font = font;
    button_update_text_position(button);
    return button;
}

button_t* button_create_size_color(char* text, int x, int y, void (*onclick)(), int width, int height,
        SDL_Color foreground_color, SDL_Color background_color, SDL_Color hover_background_color, SDL_Color click_background_color) {
    return button_create_size_color_font(text, x, y, onclick, width, height,
            foreground_color, background_color, hover_background_color, click_background_color,
            default_font);
}

button_t* button_create_size_font(char* text, int x, int y, void (*onclick)(), int width, int height, font_t* font) {
    return button_create_size_color_font(text, x, y, onclick, width, height,
            default_foreground_color, default_background_color, default_hover_background_color, default_click_background_color,
            font);
}

button_t* button_create_size(char* text, int x, int y, void (*onclick)(), int width, int height) {
    return button_create_size_color_font(text, x, y, onclick, width, height,
            default_foreground_color, default_background_color, default_hover_background_color, default_click_background_color,
            default_font);
}

button_t* button_create_font(char* text, int x, int y, void (*onclick)(), font_t* font) {
    return button_create_size_color_font(text, x, y, onclick, default_width, default_height,
            default_foreground_color, default_background_color, default_hover_background_color, default_click_background_color,
            font);
}

button_t* button_create(char* text, int x, int y, void (*onclick)()) {
    return button_create_size_color_font(text, x, y, onclick, default_width, default_height,
            default_foreground_color, default_background_color, default_hover_background_color, default_click_background_color,
            default_font);
}

bool button_hover(button_t* this, int x, int y) {
    return this->x < x && this->x + this->w > x && this->y < y && this->y + this->h > y;
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
    fill_rectangle(this->x, this->y, this->x + this->w, this->y + this->h);
    
    // draw text
    glColor4ub(this->foreground_color.r, this->foreground_color.g, this->foreground_color.b, this->foreground_color.a);
    draw_text_font(this->text_x, this->text_y, this->text, this->font);
}

void button_click(button_t* this) {
    if(button_hover(this, mouse.click_start.x, mouse.click_start.y) && 
            button_hover(this, mouse.x, mouse.y) &&
            this->onclick != NULL) {
        this->onclick();
    }
}