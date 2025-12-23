#ifndef HADIKPOS_MENU_H
#define HADIKPOS_MENU_H

#endif //HADIKPOS_MENU_H
#include "console.h"
#include <conio.h>

#define BUTTON_WIDTH 10
#define BUTTON_HEIGHT 3
#define BUTTON_COLOR 17
#define ACTIVE_BUTTON 21
#define BUTTON_COUNT 3


typedef struct Button {
    bool isActive;
} button_t;

typedef struct Menu {
    button_t buttons[BUTTON_COUNT];
    int menuType;
} menu_t;

void DrawMenu();
int UpdateMenu(menu_t * menu,char key);
void CreateButtons(menu_t * menu);