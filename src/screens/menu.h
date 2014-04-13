/* 
 * File:   menu.h
 * Author: jts
 *
 * Created on 10 Nov 2013
 */

#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H


#include "../screen.h"
#include "../main.h"
#include "../list.h"
#include "../button.h"
#include "../graphics/colour.h"

extern screen_t* screen_menu();

struct {
    list_t* buttons;
} menu_data;

#endif /* MENU_H */
