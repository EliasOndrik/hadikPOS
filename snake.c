#include "snake.h"

void SetSnakeMap(snake_map_t *snake_map) {
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j = 0; j < MAX_HEIGHT; j++) {
            snake_map->map[i][j] = ' ';
        }
    }

}

void DrawSnakeMap(client_snake_t *snake_map) {
    ResetTextEffect();
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j = 0; j < MAX_HEIGHT; j++) {
            DrawDot(X_OFFSET + i,Y_OFFSET + j);
        }
    }
    SetBackgroundColor(6);
    if (snake_map->gameSize != BIG) {
        DrawRect(AbsoluteOffestX(&snake_map->gameSize)-1, AbsoluteOffsetY(&snake_map->gameSize)-1,GetSize(&snake_map->gameSize)+2,GetSize(&snake_map->gameSize)+2);
    }
    ResetTextEffect();
}

void PlaceSnakeOnMap(snake_map_t *snake_map, int playerIndex) {
    snake_map->clientSnake[playerIndex].snake.size = 3;
    snake_map->clientSnake[playerIndex].snake.time = 0;
    switch (snake_map->clientSnake[playerIndex].snake.playerNumber) {
        case 1:
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.headPos, 1,3);
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.tailPos,1,1);
            for (int i = 1; i <4;i++) {
                position_t relative;
                RelativePosition(&relative, 1,i, &snake_map->clientSnake[playerIndex].gameSize);
                snake_map->map[relative.x][relative.y] = 'V';
            }
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.direction,0,1);
            break;
        case 2:
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.headPos, 3,GetSize(&snake_map->clientSnake[playerIndex].gameSize)-2);
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.tailPos,1,GetSize(&snake_map->clientSnake[playerIndex].gameSize)-2);
            for (int i = 1; i <4;i++) {
                position_t relative;
                RelativePosition(&relative, i,(GetSize(&snake_map->clientSnake[playerIndex].gameSize))-2, &snake_map->clientSnake[playerIndex].gameSize);
                snake_map->map[relative.x][relative.y] = '>';
            }
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.direction,1,0);
            break;
        case 3:
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.headPos, GetSize(&snake_map->clientSnake[playerIndex].gameSize)-4,1);
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.tailPos,GetSize(&snake_map->clientSnake[playerIndex].gameSize)-2,1);
            for (int i = 1; i <4;i++) {
                position_t relative;
                RelativePosition(&relative, (GetSize(&snake_map->clientSnake[playerIndex].gameSize)) -1 -i,1, &snake_map->clientSnake[playerIndex].gameSize);
                snake_map->map[relative.x][relative.y] = '<';
            }
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.direction,-1,0);
            break;
        case 4:
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.headPos, GetSize(&snake_map->clientSnake[playerIndex].gameSize)-2,GetSize(&snake_map->clientSnake[playerIndex].gameSize)-4);
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.tailPos,GetSize(&snake_map->clientSnake[playerIndex].gameSize)-2,GetSize(&snake_map->clientSnake[playerIndex].gameSize)-2);
            for (int i = 1; i <4;i++) {
                position_t relative;
                RelativePosition(&relative, (GetSize(&snake_map->clientSnake[playerIndex].gameSize))-2,(GetSize(&snake_map->clientSnake[playerIndex].gameSize))-1-i, &snake_map->clientSnake[playerIndex].gameSize);
                snake_map->map[relative.x][relative.y] = 'A';
            }
            SetPositionXY(&snake_map->clientSnake[playerIndex].snake.direction,0,-1);
            break;
        default: ;
    }
    snake_map->clientSnake[playerIndex].snake.isAlive = true;
    snake_map->clientSnake[playerIndex].snake.isActive = true;
    ResetTextEffect();
    UpdateApple(snake_map, playerIndex);

}

void DrawSnakeOnMap(client_snake_t *snake_map) {
    SetBackgroundColor(GetSnakeBodyColor(snake_map->snake.playerNumber));
    switch (snake_map->snake.playerNumber) {
        case 1:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                AbsolutePosition(&absolute, 1,i, &snake_map->gameSize);
                DrawDot(absolute.x,absolute.y);
            }
            break;
        case 2:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                AbsolutePosition(&absolute, i,(GetSize(&snake_map->gameSize))-2, &snake_map->gameSize);
                DrawDot(absolute.x,absolute.y);
            }
            break;
        case 3:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                AbsolutePosition(&absolute, (GetSize(&snake_map->gameSize)) -1 -i,1, &snake_map->gameSize);
                DrawDot(absolute.x,absolute.y);
            }
            break;
        case 4:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                AbsolutePosition(&absolute, (GetSize(&snake_map->gameSize))-2,(GetSize(&snake_map->gameSize))-1-i, &snake_map->gameSize);
                DrawDot(absolute.x,absolute.y);
            }
            break;
        default: ;
    }
}

void Update(snake_map_t *snake_map, int playerIndex) {
    if (snake_map->clientSnake[playerIndex].snake.isActive) {
        position_t relative;
        position_t newPosition;
        SetPositionXY(&newPosition,snake_map->clientSnake[playerIndex].snake.headPos.x + snake_map->clientSnake[playerIndex].snake.direction.x,snake_map->clientSnake[playerIndex].snake.headPos.y + snake_map->clientSnake[playerIndex].snake.direction.y);
        position_t currentPosition;
        SetPositionTo(&currentPosition, &snake_map->clientSnake[playerIndex].snake.headPos);
        RelativePosition(&relative, currentPosition.x, currentPosition.y, &snake_map->clientSnake[playerIndex].gameSize);
        snake_map->map[relative.x][relative.y] = DirToChar(&snake_map->clientSnake[playerIndex].snake.direction);
        bool ateApple = false;
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (PositionEquals(&newPosition, &snake_map->clientSnake[i].apple) && snake_map->clientSnake[i].snake.isActive) {
                snake_map->clientSnake[playerIndex].snake.size++;
                UpdateApple(snake_map, i);
                ateApple = true;
                break;
            }
        }
        if (!ateApple){
            SetPositionTo(&currentPosition, &snake_map->clientSnake[playerIndex].snake.tailPos);
            SetPositionTo(&snake_map->clientSnake[playerIndex].snake.tailHelp, &currentPosition);
            RelativePosition(&relative, currentPosition.x,currentPosition.y, &snake_map->clientSnake[playerIndex].gameSize);
            char ch = snake_map->map[relative.x][relative.y];
            switch (ch) {
                case '>' :
                    SetPositionXY(&currentPosition, currentPosition.x + 1, currentPosition.y);
                    break;
                case '<' :
                    SetPositionXY(&currentPosition, currentPosition.x - 1, currentPosition.y);
                    break;
                case 'A' :
                    SetPositionXY(&currentPosition, currentPosition.x, currentPosition.y - 1);
                    break;
                case 'V' :
                    SetPositionXY(&currentPosition, currentPosition.x, currentPosition.y + 1);
                    break;
                default:
                    return;

            }
            snake_map->map[relative.x][relative.y] = ' ';
            if (snake_map->clientSnake[playerIndex].gameType == LOOP) {
                LoopPosition(&currentPosition,&snake_map->clientSnake[playerIndex].gameSize);
            }
            SetPositionTo(&snake_map->clientSnake[playerIndex].snake.tailPos,&currentPosition);
        }

        switch (snake_map->clientSnake[playerIndex].gameType) {
            case LOOP :
                LoopPosition(&newPosition,&snake_map->clientSnake[playerIndex].gameSize);
                break ;
            case EDGE :
                if ((newPosition.x < 0 || newPosition.x >= GetSize(&snake_map->clientSnake[playerIndex].gameSize) || newPosition.y <0 || newPosition.y >= GetSize(&snake_map->clientSnake[playerIndex].gameSize))) {
                    snake_map->clientSnake[playerIndex].snake.isAlive = false;
                    return;
                }
                break ;
            default:
                break;
        }
        if (snake_map->clientSnake[playerIndex].timeout != 0 && (snake_map->clientSnake[playerIndex].timeout <= snake_map->clientSnake[playerIndex].snake.time/10)) {
            snake_map->clientSnake[playerIndex].snake.isAlive = false;
            return;
        }
        RelativePosition(&relative, newPosition.x,newPosition.y, &snake_map->clientSnake[playerIndex].gameSize);
        if (!(snake_map->map[relative.x][relative.y] == ' ' || snake_map->map[relative.x][relative.y] == 'a')) {
            snake_map->clientSnake[playerIndex].snake.isAlive = false;
            return;
        }
        SetPositionTo(&snake_map->clientSnake[playerIndex].snake.headPos, &newPosition);

        snake_map->map[relative.x][relative.y] = DirToChar(&snake_map->clientSnake[playerIndex].snake.direction);
        snake_map->clientSnake[playerIndex].snake.time++;

    }
}

void Draw(client_snake_t *snake_map) {
    if (snake_map->snake.isActive) {
        position_t absolute;

        position_t previousPosition;
        SetPositionXY(&previousPosition,snake_map->snake.headPos.x - snake_map->snake.direction.x,snake_map->snake.headPos.y - snake_map->snake.direction.y);
        LoopPosition(&previousPosition,&snake_map->gameSize);
        position_t currentPosition;
        SetPositionTo(&currentPosition, &snake_map->snake.headPos);
        SetBackgroundColor(GetSnakeBodyColor(snake_map->snake.playerNumber));
        AbsolutePosition(&absolute,previousPosition.x,previousPosition.y,&snake_map->gameSize);
        DrawDot(absolute.x,absolute.y);
        SetPositionTo(&currentPosition, &snake_map->snake.tailHelp);
        AbsolutePosition(&absolute, currentPosition.x,currentPosition.y, &snake_map->gameSize);
        ResetTextEffect();
        DrawDot(absolute.x,absolute.y);

        AbsolutePosition(&absolute, snake_map->snake.headPos.x, snake_map->snake.headPos.y, &snake_map->gameSize);
        SetBackgroundColor(GetSnakeHeadColor(snake_map->snake.playerNumber));
        DrawDot(absolute.x,absolute.y);

        DrawSnakeStats(&snake_map->snake);
    }
}

void UpdateApple(snake_map_t *snake_map, int playerIndex) {
    position_t relative;
    SetPositionXY(&snake_map->clientSnake[playerIndex].apple,rand()%GetSize(&snake_map->clientSnake[playerIndex].gameSize), rand()%GetSize(&snake_map->clientSnake[playerIndex].gameSize));
    RelativePosition(&relative,snake_map->clientSnake[playerIndex].apple.x,snake_map->clientSnake[playerIndex].apple.y, &snake_map->clientSnake[playerIndex].gameSize);
    while (snake_map->map[relative.x][relative.y] != ' ') {
        SetPositionXY(&snake_map->clientSnake[playerIndex].apple,rand()%GetSize(&snake_map->clientSnake[playerIndex].gameSize), rand()%GetSize(&snake_map->clientSnake[playerIndex].gameSize));
        RelativePosition(&relative,snake_map->clientSnake[playerIndex].apple.x,snake_map->clientSnake[playerIndex].apple.y, &snake_map->clientSnake[playerIndex].gameSize);
    }
    snake_map->map[relative.x][relative.y] = 'a';
}

void DrawApple(client_snake_t *snake_map) {
    position_t absolute;
    SetBackgroundColor(1);
    AbsolutePosition(&absolute, snake_map->apple.x,snake_map->apple.y, &snake_map->gameSize);
    DrawDot(absolute.x, absolute.y);
    ResetTextEffect();
}

void SetPositionTo(position_t *position, position_t const *newPosition) {
    position->x = newPosition->x;
    position->y = newPosition->y;
}

void SetPositionXY(position_t *position, int x, int y) {
    position->x = x;
    position->y = y;
}

bool PositionEquals(position_t const *position, position_t const *newPosition) {
    return position->x == newPosition->x && position->y == newPosition->y;
}

char DirToChar(position_t const *direction) {
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

position_t CharToDir(char const *direction) {
    position_t dir;
    switch (*direction) {
        case '>' :
            SetPositionXY(&dir, 1,0);
            break;
        case '<' :
            SetPositionXY(&dir,-1,0);
            break;
        case 'V' :
            SetPositionXY(&dir,0,1);
            break;
        default :
            SetPositionXY(&dir,0,-1);
    }
    return dir;
}

void LoopPosition(position_t *position, game_size_t const * gameSize) {
    if (position->x < 0) {
        position->x = GetSize(gameSize) - 1;
    }
    if (position->x >= GetSize(gameSize)) {
        position->x = 0;
    }
    if (position->y < 0) {
        position->y = GetSize(gameSize) - 1;
    }
    if (position->y >= GetSize(gameSize)) {
        position->y = 0;
    }
}

int GetSize(game_size_t const *gameSize) {
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

int AbsoluteOffestX(game_size_t const *gameSize) {
    return ((CONSOLE_WIDTH-GetSize(gameSize))/2);
}

int AbsoluteOffsetY(game_size_t const *gameSize) {
    return ((CONSOLE_HEIGHT-GetSize(gameSize))/2);
}

void AbsolutePosition(position_t *position, int x, int y, game_size_t const *gameSize) {
    position->x = AbsoluteOffestX(gameSize) + x;
    position->y = AbsoluteOffsetY(gameSize) + y;
}

int RelativeOffsetX(game_size_t const *gameSize) {
    return (MAX_WIDTH-GetSize(gameSize))/2;
}

int RelativeOffsetY(game_size_t const *gameSize) {
    return (MAX_HEIGHT-GetSize(gameSize))/2;
}

void RelativePosition(position_t *position, int x, int y, game_size_t const *gameSize) {
    position->x = RelativeOffsetX(gameSize) + x;
    position->y = RelativeOffsetY(gameSize) + y;
}

void DrawSnakeStats(snake_t *snake) {
    position_t position;
    switch (snake->playerNumber) {
        case 1:
            SetPositionXY(&position, 2,2);
            break;
        case 2:
            SetPositionXY(&position, 2,CONSOLE_HEIGHT - 7);
            break;
        case 3:
            SetPositionXY(&position, CONSOLE_WIDTH - 13 ,2);
            break;
        case 4:
            SetPositionXY(&position, CONSOLE_WIDTH - 13,CONSOLE_HEIGHT - 7);
            break;
        default:
            break;
    }
    ResetTextEffect();
    MoveTo(position.x*2+3,position.y+2);
    printf("Player %d", snake->playerNumber);
    MoveTo(position.x*2+3,position.y+3);
    printf("Size: %d   ", snake->size);
    MoveTo(position.x*2+3,position.y+4);
    printf("Time: %d    ", snake->time/10);
}

void ToString(client_snake_t const *snakeMap, char * data) {
    int alive  = snakeMap->snake.isAlive;
    int active = snakeMap->snake.isActive;
    sprintf(data,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
        snakeMap->gameSize,snakeMap->timeout,
        snakeMap->snake.playerNumber,
        snakeMap->snake.headPos.x, snakeMap->snake.headPos.y,
        snakeMap->snake.direction.x, snakeMap->snake.direction.y,
        snakeMap->snake.tailPos.x, snakeMap->snake.tailPos.y,
        snakeMap->snake.tailHelp.x, snakeMap->snake.tailHelp.y,
        snakeMap->snake.size, snakeMap->snake.time,
        snakeMap->apple.x, snakeMap->apple.y,
        alive,active);
}

int ReadString(char const *data, client_snake_t * snake) {
    int alive, active;
    bool success = sscanf(data,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
        &snake->gameSize,&snake->timeout,
        &snake->snake.playerNumber,
        &snake->snake.headPos.x, &snake->snake.headPos.y,
        &snake->snake.direction.x, &snake->snake.direction.y,
        &snake->snake.tailPos.x, &snake->snake.tailPos.y,
        &snake->snake.tailHelp.x, &snake->snake.tailHelp.y,
        &snake->snake.size, &snake->snake.time,
        &snake->apple.x, &snake->apple.y,
        &alive, &active) == 17;
    snake->snake.isAlive = alive == 1;
    snake->snake.isActive = active == 1;
    return success;
}

void GiveServerString(client_snake_t const *snakeMap, char *data) {
    int alive  = snakeMap->snake.isAlive;
    int active = snakeMap->snake.isActive;
    sprintf(data,"%d;%d;%d;%d;%d;%d;%d;%d;",
        snakeMap->gameSize,snakeMap->gameType,
        snakeMap->timeout,
        snakeMap->snake.playerNumber,
        snakeMap->snake.direction.x, snakeMap->snake.direction.y,
        alive, active
        );
}

int ServerReadString(char const *data, client_snake_t *snake) {
    int alive, active;
    bool success = sscanf(data,"%d;%d;%d;%d;%d;%d;%d;%d;",
        &snake->gameSize,&snake->gameType,
        &snake->timeout,
        &snake->snake.playerNumber,
        &snake->snake.direction.x, &snake->snake.direction.y,
        &alive, &active) == 8;
    snake->snake.isAlive = alive == 1;
    snake->snake.isActive = active == 1;
    return success;
}

int GetSnakeHeadColor(int playerNumber) {
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

int GetSnakeBodyColor(int playerNumber) {
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

