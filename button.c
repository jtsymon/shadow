
#include <SDL2/SDL.h>

#include "button.h"


const int default_width = 100;
const int default_height = 50;
const SDL_Color default_foreground_color = { 50, 0, 0, 255 };
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

void draw_text(int x, int y, int width, int height, char* text, SDL_Color color) {
    SDL_Surface* text_surface = TTF_RenderText_Solid(RENDER.font, text, color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(RENDER.renderer, text_surface);
    if(width > 0) {
        x += (width - text_surface->w) / 2;
    }
    if(height > 0) {
        y += (height - text_surface->h) / 2;
    }
    const SDL_Rect bounds = { x, y, text_surface->w, text_surface->h };
    SDL_FreeSurface(text_surface);
    SDL_RenderCopy(RENDER.renderer, text_texture, NULL, &bounds);
    SDL_DestroyTexture(text_texture);
}

void button_draw(button_t* this) {
    // draw background
    if(mouse.buttons[SDL_BUTTON_LEFT] && button_hover(this, mouse.click_start.x, mouse.click_start.y)) {
        boxRGBA(RENDER.renderer, this->x, this->y, this->x + this->width, this->y + this->height,
                this->click_background_color.r, this->click_background_color.g, this->click_background_color.b, this->click_background_color.a);
    } else if(button_hover(this, mouse.x, mouse.y)) {
        boxRGBA(RENDER.renderer, this->x, this->y, this->x + this->width, this->y + this->height,
                this->hover_background_color.r, this->hover_background_color.g, this->hover_background_color.b, this->hover_background_color.a);
    } else {
        boxRGBA(RENDER.renderer, this->x, this->y, this->x + this->width, this->y + this->height,
                this->background_color.r, this->background_color.g, this->background_color.b, this->background_color.a);
    }
    
    // draw text
    draw_text(this->x, this->y, this->width, this->height, this->text, this->foreground_color);
}

void button_click(button_t* this) {
    if(button_hover(this, mouse.click_start.x, mouse.click_start.y) && 
            button_hover(this, mouse.x, mouse.y) &&
            this->onclick != NULL) {
        this->onclick();
    }
}