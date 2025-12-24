#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include "console.h"
#include "menu.h"


int main(void) {
    srand(time(NULL));
    ClearScreen();
    SetCursorVisibility(false);
    DrawMenu();
    snake_map_t snake_map;
    menu_t menu;
    CreateButtons(&menu);
    SetSnakeMap(&snake_map);
    DrawSnakeMap(&snake_map);
    PlaceSnakeOnMap(&snake_map);
    DrawSnakeOnMap(&snake_map);
    UpdateMenu(&menu,' ');
    snake_map.snake.isActive = false;
    while (1) {
        if (kbhit()) {
            char key = (char)getch();
            if (key == 'w' && snake_map.snake.direction.y != 1) {
                SetPositionXY(&snake_map.snake.direction,0,-1);
            }
            if (key == 's' && snake_map.snake.direction.y != -1) {
                SetPositionXY(&snake_map.snake.direction,0,+1);
            }
            if (key == 'a' && snake_map.snake.direction.x != 1) {
                SetPositionXY(&snake_map.snake.direction,-1,0);
            }
            if (key == 'd' && snake_map.snake.direction.x != -1) {
                SetPositionXY(&snake_map.snake.direction,+1,0);
            }
            int state = UpdateMenu(&menu,key);
            if (state == -1) {
                break;
            }
            if (state == 0) {
                snake_map.snake.isActive = true;
            } else {
                snake_map.snake.isActive = false;
                snake_map.gameSize = menu.gameSize;
                snake_map.gameType = menu.gameType;
                snake_map.snake.isAlive = false;
            }
        }
        if (snake_map.snake.isActive) {
            if (!snake_map.snake.isAlive) {
                SetSnakeMap(&snake_map);
                DrawSnakeMap(&snake_map);
                PlaceSnakeOnMap(&snake_map);
                DrawSnakeOnMap(&snake_map);
            }
            Update(&snake_map);
            Draw(&snake_map);
            DrawApple(&snake_map);
        }

        usleep(100*1000);

    }
    ResetTextEffect();
    SetCursorVisibility(true);
    ClearScreen();
    return 0;
}
