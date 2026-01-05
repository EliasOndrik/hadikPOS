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
    snake_map_t snakeMap;
    InitializeMap(&snakeMap);
    menu_t menu;
    InitializeMenu(&menu);
    CreateButtons(&menu);
    SetSnakeMap(&snakeMap);
    DrawSnakeMap(&snakeMap.clientSnake[0]);
    snakeMap.clientSnake[0].snake.playerNumber = 1;
    UpdateMenu(&menu,' ');
    snakeMap.clientSnake[0].snake.isActive = false;

    while (1) {
        if (kbhit()) {

            char key = (char)getch();
            int state = UpdateMenu(&menu,key);
            if (state == -1) {
                break;
            }
            if (state == 0) {
                if (key == 'w' && snakeMap.clientSnake[0].snake.direction.y != 1) {
                    SetPositionXY(&snakeMap.clientSnake[0].snake.direction,0,-1);
                }
                if (key == 's' && snakeMap.clientSnake[0].snake.direction.y != -1) {
                    SetPositionXY(&snakeMap.clientSnake[0].snake.direction,0,+1);
                }
                if (key == 'a' && snakeMap.clientSnake[0].snake.direction.x != 1) {
                    SetPositionXY(&snakeMap.clientSnake[0].snake.direction,-1,0);
                }
                if (key == 'd' && snakeMap.clientSnake[0].snake.direction.x != -1) {
                    SetPositionXY(&snakeMap.clientSnake[0].snake.direction,+1,0);
                }
                snakeMap.clientSnake[0].snake.isActive = true;
            } else if (state == 4) {
                snakeMap.clientSnake[0].snake.isActive = false;
            } else if (state == 7 || state == 6 || state == 3) {
                UpdateMenu(&menu,' ');
                snakeMap.clientSnake[0].snake.isActive = true;
            } else {
                snakeMap.clientSnake[0].snake.isActive = false;
                snakeMap.clientSnake[0].snake.isAlive = false;
                snakeMap.clientSnake[0].gameSize = menu.gameSize;
                snakeMap.clientSnake[0].gameType = menu.gameType;
                snakeMap.clientSnake[0].timeout = menu.time;
                snakeMap.maxPlayerCount = menu.playerCount;
            }
        }
        if (snakeMap.clientSnake[0].snake.isActive) {
            if (!snakeMap.clientSnake[0].snake.isAlive) {
                SetSnakeMap(&snakeMap);
                DrawSnakeMap(&snakeMap.clientSnake[0]);
                PlaceSnakeOnMap(&snakeMap, 0);
                DrawSnakeOnMap(&snakeMap.clientSnake[0]);
            }
            Update(&snakeMap,0);
            Draw(&snakeMap.clientSnake[0]);
            DrawApple(&snakeMap.clientSnake[0]);
        }

        usleep(100*1000);

    }
    ResetTextEffect();
    SetCursorVisibility(true);
    ClearScreen();
    return 0;
}
