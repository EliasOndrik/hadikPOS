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
    DrawSnakeMap(&snake_map.clientSnake);
    snake_map.clientSnake.snake.playerNumber = 1;
    PlaceSnakeOnMap(&snake_map);
    DrawSnakeOnMap(&snake_map.clientSnake);
    UpdateMenu(&menu,' ');
    snake_map.clientSnake.snake.isActive = false;

    while (1) {
        if (kbhit()) {

            char key = (char)getch();
            int state = UpdateMenu(&menu,key);
            if (state == -1) {
                break;
            }
            if (state == 0) {
                if (key == 'w' && snake_map.clientSnake.snake.direction.y != 1) {
                    SetPositionXY(&snake_map.clientSnake.snake.direction,0,-1);
                }
                if (key == 's' && snake_map.clientSnake.snake.direction.y != -1) {
                    SetPositionXY(&snake_map.clientSnake.snake.direction,0,+1);
                }
                if (key == 'a' && snake_map.clientSnake.snake.direction.x != 1) {
                    SetPositionXY(&snake_map.clientSnake.snake.direction,-1,0);
                }
                if (key == 'd' && snake_map.clientSnake.snake.direction.x != -1) {
                    SetPositionXY(&snake_map.clientSnake.snake.direction,+1,0);
                }
                snake_map.clientSnake.snake.isActive = true;
            } else if (state == 4) {
                snake_map.clientSnake.snake.isActive = false;
            } else {
                snake_map.clientSnake.snake.isActive = false;
                snake_map.clientSnake.gameSize = menu.gameSize;
                snake_map.clientSnake.gameType = menu.gameType;
                snake_map.clientSnake.timeout = menu.time;
                snake_map.clientSnake.snake.isAlive = false;
            }
        }
        if (snake_map.clientSnake.snake.isActive) {
            if (!snake_map.clientSnake.snake.isAlive) {
                SetSnakeMap(&snake_map);
                DrawSnakeMap(&snake_map.clientSnake);
                PlaceSnakeOnMap(&snake_map);
                DrawSnakeOnMap(&snake_map.clientSnake);
            }
            Update(&snake_map);
            Draw(&snake_map.clientSnake);
            DrawApple(&snake_map.clientSnake);
        }

        usleep(100*1000);

    }
    ResetTextEffect();
    SetCursorVisibility(true);
    ClearScreen();
    return 0;
}
