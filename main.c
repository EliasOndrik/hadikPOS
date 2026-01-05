#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include "console.h"
#include "menu.h"


int main(void) {
    srand(time(NULL));
    clear_screen();
    set_cursor_visibility(false);
    draw_menu();
    snake_map_t snakeMap;
    initialize_map(&snakeMap);
    menu_t menu;
    initialize_menu(&menu);
    create_buttons(&menu);
    set_snake_map(&snakeMap);
    draw_snake_map(&snakeMap.clientSnake[0]);
    snakeMap.clientSnake[0].snake.playerNumber = 1;
    update_menu(&menu,' ');
    snakeMap.clientSnake[0].snake.isActive = false;

    while (1) {
        if (kbhit()) {

            char key = (char)getch();
            int state = update_menu(&menu,key);
            if (state == -1) {
                break;
            }
            if (state == 0) {
                if (key == 'w' && snakeMap.clientSnake[0].snake.direction.y != 1) {
                    set_position_xy(&snakeMap.clientSnake[0].snake.direction,0,-1);
                }
                if (key == 's' && snakeMap.clientSnake[0].snake.direction.y != -1) {
                    set_position_xy(&snakeMap.clientSnake[0].snake.direction,0,+1);
                }
                if (key == 'a' && snakeMap.clientSnake[0].snake.direction.x != 1) {
                    set_position_xy(&snakeMap.clientSnake[0].snake.direction,-1,0);
                }
                if (key == 'd' && snakeMap.clientSnake[0].snake.direction.x != -1) {
                    set_position_xy(&snakeMap.clientSnake[0].snake.direction,+1,0);
                }
                snakeMap.clientSnake[0].snake.isActive = true;
            } else if (state == 4) {
                snakeMap.clientSnake[0].snake.isActive = false;
            } else if (state == 7 || state == 6 || state == 3) {
                update_menu(&menu,' ');
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
                set_snake_map(&snakeMap);
                draw_snake_map(&snakeMap.clientSnake[0]);
                place_snake_on_map(&snakeMap, 0);
                draw_snake_on_map(&snakeMap.clientSnake[0]);
            }
            update(&snakeMap,0);
            draw(&snakeMap.clientSnake[0]);
            draw_apple(&snakeMap.clientSnake[0]);
        }

        usleep(100*1000);

    }
    reset_text_effect();
    set_cursor_visibility(true);
    clear_screen();
    return 0;
}
