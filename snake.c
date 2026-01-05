#include "snake.h"

void initialize_map(snake_map_t *snakeMap) {
    for (int i = 0; i <MAX_PLAYERS; i++) {
        initialize_snake(&snakeMap->clientSnake[i]);
    }
    snakeMap->maxPlayerCount = 1;
}

void initialize_snake(client_snake_t *client) {
    client->snake.isActive = false;
    client->snake.isAlive = false;
    client->snake.playerNumber = -1;
    client->snake.time = 0;
    set_position_xy(&client->snake.direction, 0,0);
    set_position_xy(&client->snake.headPos, -1,-1);
    set_position_xy(&client->snake.tailPos, -1,-1);
    set_position_xy(&client->snake.tailHelp, -1,-1);
    client->snake.size = 0;
    client->gameSize = BIG;
    client->gameType = LOOP;
    set_position_xy(&client->apple, -1,-1);
    client->timeout = 0;

}

void set_snake_map(snake_map_t *snakeMap) {
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j = 0; j < MAX_HEIGHT; j++) {
            snakeMap->map[i][j] = ' ';
        }
    }

}

void draw_snake_map(client_snake_t *client) {
    reset_text_effect();
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j = 0; j < MAX_HEIGHT; j++) {
            draw_dot(X_OFFSET + i,Y_OFFSET + j);
        }
    }
    set_background_color(6);
    if (client->gameSize != BIG) {
        draw_rect(absolute_offest_x(&client->gameSize)-1, absolute_offset_y(&client->gameSize)-1,get_size(&client->gameSize)+2,get_size(&client->gameSize)+2);
    }
    reset_text_effect();
}

void place_snake_on_map(snake_map_t *snakeMap, int playerIndex) {
    snakeMap->clientSnake[playerIndex].snake.size = 3;
    snakeMap->clientSnake[playerIndex].snake.time = 0;
    switch (snakeMap->clientSnake[playerIndex].snake.playerNumber) {
        case 1:
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.headPos, 1,3);
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.tailPos,1,1);
            for (int i = 1; i <4;i++) {
                position_t relative;
                relative_position(&relative, 1,i, &snakeMap->clientSnake[playerIndex].gameSize);
                snakeMap->map[relative.x][relative.y] = 'V';
            }
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.direction,0,1);
            break;
        case 2:
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.headPos, 3,get_size(&snakeMap->clientSnake[playerIndex].gameSize)-2);
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.tailPos,1,get_size(&snakeMap->clientSnake[playerIndex].gameSize)-2);
            for (int i = 1; i <4;i++) {
                position_t relative;
                relative_position(&relative, i,(get_size(&snakeMap->clientSnake[playerIndex].gameSize))-2, &snakeMap->clientSnake[playerIndex].gameSize);
                snakeMap->map[relative.x][relative.y] = '>';
            }
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.direction,1,0);
            break;
        case 3:
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.headPos, get_size(&snakeMap->clientSnake[playerIndex].gameSize)-4,1);
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.tailPos,get_size(&snakeMap->clientSnake[playerIndex].gameSize)-2,1);
            for (int i = 1; i <4;i++) {
                position_t relative;
                relative_position(&relative, (get_size(&snakeMap->clientSnake[playerIndex].gameSize)) -1 -i,1, &snakeMap->clientSnake[playerIndex].gameSize);
                snakeMap->map[relative.x][relative.y] = '<';
            }
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.direction,-1,0);
            break;
        case 4:
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.headPos, get_size(&snakeMap->clientSnake[playerIndex].gameSize)-2,get_size(&snakeMap->clientSnake[playerIndex].gameSize)-4);
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.tailPos,get_size(&snakeMap->clientSnake[playerIndex].gameSize)-2,get_size(&snakeMap->clientSnake[playerIndex].gameSize)-2);
            for (int i = 1; i <4;i++) {
                position_t relative;
                relative_position(&relative, (get_size(&snakeMap->clientSnake[playerIndex].gameSize))-2,(get_size(&snakeMap->clientSnake[playerIndex].gameSize))-1-i, &snakeMap->clientSnake[playerIndex].gameSize);
                snakeMap->map[relative.x][relative.y] = 'A';
            }
            set_position_xy(&snakeMap->clientSnake[playerIndex].snake.direction,0,-1);
            break;
        default: ;
    }
    snakeMap->clientSnake[playerIndex].snake.isAlive = true;
    snakeMap->clientSnake[playerIndex].snake.isActive = true;
    reset_text_effect();
    update_apple(snakeMap, playerIndex);

}

void draw_snake_on_map(client_snake_t *client) {
    set_background_color(get_snake_body_color(client->snake.playerNumber));
    switch (client->snake.playerNumber) {
        case 1:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                absolute_position(&absolute, 1,i, &client->gameSize);
                draw_dot(absolute.x,absolute.y);
            }
            break;
        case 2:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                absolute_position(&absolute, i,(get_size(&client->gameSize))-2, &client->gameSize);
                draw_dot(absolute.x,absolute.y);
            }
            break;
        case 3:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                absolute_position(&absolute, (get_size(&client->gameSize)) -1 -i,1, &client->gameSize);
                draw_dot(absolute.x,absolute.y);
            }
            break;
        case 4:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                absolute_position(&absolute, (get_size(&client->gameSize))-2,(get_size(&client->gameSize))-1-i, &client->gameSize);
                draw_dot(absolute.x,absolute.y);
            }
            break;
        default: ;
    }
}

void update(snake_map_t *snakeMap, int playerIndex) {
    if (snakeMap->clientSnake[playerIndex].snake.isActive) {
        position_t relative;
        position_t newPosition;
        set_position_xy(&newPosition,snakeMap->clientSnake[playerIndex].snake.headPos.x + snakeMap->clientSnake[playerIndex].snake.direction.x,snakeMap->clientSnake[playerIndex].snake.headPos.y + snakeMap->clientSnake[playerIndex].snake.direction.y);
        position_t currentPosition;
        set_position_to(&currentPosition, &snakeMap->clientSnake[playerIndex].snake.headPos);
        relative_position(&relative, currentPosition.x, currentPosition.y, &snakeMap->clientSnake[playerIndex].gameSize);
        snakeMap->map[relative.x][relative.y] = dir_to_char(&snakeMap->clientSnake[playerIndex].snake.direction);
        bool ateApple = false;
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (position_equals(&newPosition, &snakeMap->clientSnake[i].apple) && snakeMap->clientSnake[i].snake.isActive) {
                snakeMap->clientSnake[playerIndex].snake.size++;
                update_apple(snakeMap, i);
                ateApple = true;
                break;
            }
        }
        if (!ateApple){
            set_position_to(&currentPosition, &snakeMap->clientSnake[playerIndex].snake.tailPos);
            set_position_to(&snakeMap->clientSnake[playerIndex].snake.tailHelp, &currentPosition);
            relative_position(&relative, currentPosition.x,currentPosition.y, &snakeMap->clientSnake[playerIndex].gameSize);
            char ch = snakeMap->map[relative.x][relative.y];
            switch (ch) {
                case '>' :
                    set_position_xy(&currentPosition, currentPosition.x + 1, currentPosition.y);
                    break;
                case '<' :
                    set_position_xy(&currentPosition, currentPosition.x - 1, currentPosition.y);
                    break;
                case 'A' :
                    set_position_xy(&currentPosition, currentPosition.x, currentPosition.y - 1);
                    break;
                case 'V' :
                    set_position_xy(&currentPosition, currentPosition.x, currentPosition.y + 1);
                    break;
                default:
                    return;

            }
            snakeMap->map[relative.x][relative.y] = ' ';
            if (snakeMap->clientSnake[playerIndex].gameType == LOOP) {
                loop_position(&currentPosition,&snakeMap->clientSnake[playerIndex].gameSize);
            }
            set_position_to(&snakeMap->clientSnake[playerIndex].snake.tailPos,&currentPosition);
        }

        switch (snakeMap->clientSnake[playerIndex].gameType) {
            case LOOP :
                loop_position(&newPosition,&snakeMap->clientSnake[playerIndex].gameSize);
                break ;
            case EDGE :
                if ((newPosition.x < 0 || newPosition.x >= get_size(&snakeMap->clientSnake[playerIndex].gameSize) || newPosition.y <0 || newPosition.y >= get_size(&snakeMap->clientSnake[playerIndex].gameSize))) {
                    snakeMap->clientSnake[playerIndex].snake.isAlive = false;
                    return;
                }
                break ;
            default:
                break;
        }
        if (snakeMap->clientSnake[playerIndex].timeout != 0 && (snakeMap->clientSnake[playerIndex].timeout <= snakeMap->clientSnake[playerIndex].snake.time/10)) {
            snakeMap->clientSnake[playerIndex].snake.isAlive = false;
            return;
        }
        relative_position(&relative, newPosition.x,newPosition.y, &snakeMap->clientSnake[playerIndex].gameSize);
        if (!(snakeMap->map[relative.x][relative.y] == ' ' || snakeMap->map[relative.x][relative.y] == 'a')) {
            snakeMap->clientSnake[playerIndex].snake.isAlive = false;
            return;
        }
        set_position_to(&snakeMap->clientSnake[playerIndex].snake.headPos, &newPosition);

        snakeMap->map[relative.x][relative.y] = dir_to_char(&snakeMap->clientSnake[playerIndex].snake.direction);
        snakeMap->clientSnake[playerIndex].snake.time++;

    }
}

void draw(client_snake_t *client) {
    if (client->snake.isActive) {
        position_t absolute;

        position_t previousPosition;
        set_position_xy(&previousPosition,client->snake.headPos.x - client->snake.direction.x,client->snake.headPos.y - client->snake.direction.y);
        loop_position(&previousPosition,&client->gameSize);
        position_t currentPosition;
        set_position_to(&currentPosition, &client->snake.headPos);
        set_background_color(get_snake_body_color(client->snake.playerNumber));
        absolute_position(&absolute,previousPosition.x,previousPosition.y,&client->gameSize);
        draw_dot(absolute.x,absolute.y);
        set_position_to(&currentPosition, &client->snake.tailHelp);
        absolute_position(&absolute, currentPosition.x,currentPosition.y, &client->gameSize);
        reset_text_effect();
        draw_dot(absolute.x,absolute.y);

        absolute_position(&absolute, client->snake.headPos.x, client->snake.headPos.y, &client->gameSize);
        set_background_color(get_snake_head_color(client->snake.playerNumber));
        draw_dot(absolute.x,absolute.y);

        draw_snake_stats(&client->snake);
    }
}

void update_apple(snake_map_t *snakeMap, int playerIndex) {
    position_t relative;
    set_position_xy(&snakeMap->clientSnake[playerIndex].apple,rand()%get_size(&snakeMap->clientSnake[playerIndex].gameSize), rand()%get_size(&snakeMap->clientSnake[playerIndex].gameSize));
    relative_position(&relative,snakeMap->clientSnake[playerIndex].apple.x,snakeMap->clientSnake[playerIndex].apple.y, &snakeMap->clientSnake[playerIndex].gameSize);
    while (snakeMap->map[relative.x][relative.y] != ' ') {
        set_position_xy(&snakeMap->clientSnake[playerIndex].apple,rand()%get_size(&snakeMap->clientSnake[playerIndex].gameSize), rand()%get_size(&snakeMap->clientSnake[playerIndex].gameSize));
        relative_position(&relative,snakeMap->clientSnake[playerIndex].apple.x,snakeMap->clientSnake[playerIndex].apple.y, &snakeMap->clientSnake[playerIndex].gameSize);
    }
    snakeMap->map[relative.x][relative.y] = 'a';
}

void draw_apple(client_snake_t *client) {
    position_t absolute;
    set_background_color(1);
    absolute_position(&absolute, client->apple.x,client->apple.y, &client->gameSize);
    draw_dot(absolute.x, absolute.y);
    reset_text_effect();
}

void set_position_to(position_t *position, position_t const *newPosition) {
    position->x = newPosition->x;
    position->y = newPosition->y;
}

void set_position_xy(position_t *position, int x, int y) {
    position->x = x;
    position->y = y;
}

bool position_equals(position_t const *position, position_t const *newPosition) {
    return position->x == newPosition->x && position->y == newPosition->y;
}

char dir_to_char(position_t const *direction) {
    if (direction->x == 1) {
        return '>';
    }
    if (direction->x == -1) {
        return '<';
    }
    if (direction->y == 1) {
        return 'V';
    }
    return 'A';
}

position_t char_to_dir(char const *direction) {
    position_t dir;
    switch (*direction) {
        case '>' :
            set_position_xy(&dir, 1,0);
            break;
        case '<' :
            set_position_xy(&dir,-1,0);
            break;
        case 'V' :
            set_position_xy(&dir,0,1);
            break;
        default :
            set_position_xy(&dir,0,-1);
    }
    return dir;
}

void loop_position(position_t *position, game_size_t const * gameSize) {
    if (position->x < 0) {
        position->x = get_size(gameSize) - 1;
    }
    if (position->x >= get_size(gameSize)) {
        position->x = 0;
    }
    if (position->y < 0) {
        position->y = get_size(gameSize) - 1;
    }
    if (position->y >= get_size(gameSize)) {
        position->y = 0;
    }
}

int get_size(game_size_t const *gameSize) {
    switch (*gameSize) {
        case SMALL:
            return 10;
        case MEDIUM:
            return 19;
        case BIG:
            return MAX_WIDTH;
    }
    return MAX_WIDTH;
}

int absolute_offest_x(game_size_t const *gameSize) {
    return ((CONSOLE_WIDTH-get_size(gameSize))/2);
}

int absolute_offset_y(game_size_t const *gameSize) {
    return ((CONSOLE_HEIGHT-get_size(gameSize))/2);
}

void absolute_position(position_t *position, int x, int y, game_size_t const *gameSize) {
    position->x = absolute_offest_x(gameSize) + x;
    position->y = absolute_offset_y(gameSize) + y;
}

int relative_offset_x(game_size_t const *gameSize) {
    return (MAX_WIDTH-get_size(gameSize))/2;
}

int relative_offset_y(game_size_t const *gameSize) {
    return (MAX_HEIGHT-get_size(gameSize))/2;
}

void relative_position(position_t *position, int x, int y, game_size_t const *gameSize) {
    position->x = relative_offset_x(gameSize) + x;
    position->y = relative_offset_y(gameSize) + y;
}

void draw_snake_stats(snake_t *snake) {
    position_t position;
    switch (snake->playerNumber) {
        case 1:
            set_position_xy(&position, 2,2);
            break;
        case 2:
            set_position_xy(&position, 2,CONSOLE_HEIGHT - 7);
            break;
        case 3:
            set_position_xy(&position, CONSOLE_WIDTH - 13 ,2);
            break;
        case 4:
            set_position_xy(&position, CONSOLE_WIDTH - 13,CONSOLE_HEIGHT - 7);
            break;
        default:
            break;
    }
    reset_text_effect();
    move_to(position.x*2+3,position.y+2);
    printf("Player %d", snake->playerNumber);
    move_to(position.x*2+3,position.y+3);
    printf("Size: %d   ", snake->size);
    move_to(position.x*2+3,position.y+4);
    printf("Time: %d    ", snake->time/10);
}

void to_string(client_snake_t const *client, char * data) {
    int alive  = client->snake.isAlive;
    int active = client->snake.isActive;
    sprintf(data,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
        client->gameSize,client->timeout,
        client->snake.playerNumber,
        client->snake.headPos.x, client->snake.headPos.y,
        client->snake.direction.x, client->snake.direction.y,
        client->snake.tailPos.x, client->snake.tailPos.y,
        client->snake.tailHelp.x, client->snake.tailHelp.y,
        client->snake.size, client->snake.time,
        client->apple.x, client->apple.y,
        alive,active);
}

int read_string(char const *data, client_snake_t * client) {
    int alive, active;
    bool success = sscanf(data,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
        &client->gameSize,&client->timeout,
        &client->snake.playerNumber,
        &client->snake.headPos.x, &client->snake.headPos.y,
        &client->snake.direction.x, &client->snake.direction.y,
        &client->snake.tailPos.x, &client->snake.tailPos.y,
        &client->snake.tailHelp.x, &client->snake.tailHelp.y,
        &client->snake.size, &client->snake.time,
        &client->apple.x, &client->apple.y,
        &alive, &active) == 17;
    client->snake.isAlive = alive == 1;
    client->snake.isActive = active == 1;
    return success;
}

void give_server_string(client_snake_t const *client, char *data) {
    int alive  = client->snake.isAlive;
    int active = client->snake.isActive;
    sprintf(data,"%d;%d;%d;%d;%d;%d;%d;%d;",
        client->gameSize,client->gameType,
        client->timeout,
        client->snake.playerNumber,
        client->snake.direction.x, client->snake.direction.y,
        alive, active
        );
}

int server_read_string(char const *data, client_snake_t *client) {
    int alive, active;
    bool success = sscanf(data,"%d;%d;%d;%d;%d;%d;%d;%d;",
        &client->gameSize,&client->gameType,
        &client->timeout,
        &client->snake.playerNumber,
        &client->snake.direction.x, &client->snake.direction.y,
        &alive, &active) == 8;
    client->snake.isAlive = alive == 1;
    client->snake.isActive = active == 1;
    return success;
}

int get_snake_head_color(int playerNumber) {
    switch (playerNumber) {
        case 1:
            return 118;
        case 2:
            return 214;
        case 3:
            return 205;
        case 4:
            return 195;
        default:
            return 0;
    }
}

int get_snake_body_color(int playerNumber) {
    switch (playerNumber) {
        case 1:
            return 2;
        case 2:
            return 3;
        case 3:
            return 5;
        case 4:
            return 7;
        default:
            return 0;
    }
}

