#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include "console.h"
#include "snake.h"

int main(void) {
    srand(time(NULL));
    ClearScreen();
    SetCursorVisibility(false);
    SetBackgroundColor(6);
    DrawRect(15,0,30,30);
    SetBackgroundColor(4);
    DrawRect(0,0,15,30);
    DrawRect( 60-15,0,15,30);
    snake_map_t snake_map;
    SetSnakeMap(&snake_map);
    PlaceSnakeOnMap(&snake_map);
    while (1) {

        if (_kbhit()) {
            char key = (char)_getch();
            if (key == 'q' || key == 27) {
                break;
            }
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
        }
        Update(&snake_map);
        usleep(100*1000);

    }
    ResetTextEffect();
    SetCursorVisibility(true);
    ClearScreen();
    return 0;
}
