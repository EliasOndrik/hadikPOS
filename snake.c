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

void PlaceSnakeOnMap(snake_map_t *snake_map) {
    snake_map->clientSnake.snake.bodyColor = 2;
    snake_map->clientSnake.snake.headColor = 118;
    snake_map->clientSnake.snake.size = 3;
    snake_map->clientSnake.snake.time = 0;
    snake_map->clientSnake.snake.playerNumber = 1;
    SetPositionXY(&snake_map->clientSnake.snake.headPos, 3,GetSize(&snake_map->clientSnake.gameSize)/2);
    SetPositionXY(&snake_map->clientSnake.snake.tailPos,1,GetSize(&snake_map->clientSnake.gameSize)/2);
    for (int i = 1; i <4;i++) {
        position_t relative;
        RelativePosition(&relative, i,(GetSize(&snake_map->clientSnake.gameSize)/2), &snake_map->clientSnake.gameSize);
        snake_map->map[relative.x][relative.y] = '>';
    }
    snake_map->clientSnake.snake.isAlive = true;
    snake_map->clientSnake.snake.isActive = true;
    ResetTextEffect();
    SetPositionXY(&snake_map->clientSnake.snake.direction,1,0);
    UpdateApple(snake_map);

}

void DrawSnakeOnMap(client_snake_t *snake_map) {
    SetBackgroundColor(snake_map->snake.bodyColor);
    for (int i = 1; i <4;i++) {
        position_t absolute;
        AbsolutePosition(&absolute, i,(GetSize(&snake_map->gameSize)/2), &snake_map->gameSize);
        DrawDot(absolute.x,absolute.y);
    }
}

void Update(snake_map_t *snake_map) {
    if (snake_map->clientSnake.snake.isActive) {
        position_t relative;
        position_t newPosition;
        SetPositionXY(&newPosition,snake_map->clientSnake.snake.headPos.x + snake_map->clientSnake.snake.direction.x,snake_map->clientSnake.snake.headPos.y + snake_map->clientSnake.snake.direction.y);
        position_t currentPosition;
        SetPositionTo(&currentPosition, &snake_map->clientSnake.snake.headPos);
        RelativePosition(&relative, currentPosition.x, currentPosition.y, &snake_map->clientSnake.gameSize);
        snake_map->map[relative.x][relative.y] = DirToChar(&snake_map->clientSnake.snake.direction);

        if (PositionEquals(&newPosition, &snake_map->clientSnake.apple)) {
            snake_map->clientSnake.snake.size++;
            UpdateApple(snake_map);
        } else {
            SetPositionTo(&currentPosition, &snake_map->clientSnake.snake.tailPos);
            SetPositionTo(&snake_map->clientSnake.snake.tailHelp, &currentPosition);
            RelativePosition(&relative, currentPosition.x,currentPosition.y, &snake_map->clientSnake.gameSize);
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
            if (snake_map->clientSnake.gameType == LOOP) {
                LoopPosition(&currentPosition,&snake_map->clientSnake.gameSize);
            }
            SetPositionTo(&snake_map->clientSnake.snake.tailPos,&currentPosition);
        }

        switch (snake_map->clientSnake.gameType) {
            case LOOP :
                LoopPosition(&newPosition,&snake_map->clientSnake.gameSize);
                break ;
            case EDGE :
                if ((newPosition.x < 0 || newPosition.x >= GetSize(&snake_map->clientSnake.gameSize) || newPosition.y <0 || newPosition.y >= GetSize(&snake_map->clientSnake.gameSize))) {
                    snake_map->clientSnake.snake.isAlive = false;
                    return;
                }
                break ;
            default:
                break;
        }

        RelativePosition(&relative, newPosition.x,newPosition.y, &snake_map->clientSnake.gameSize);
        if (!(snake_map->map[relative.x][relative.y] == ' ' || snake_map->map[relative.x][relative.y] == 'a')) {
            snake_map->clientSnake.snake.isAlive = false;
            return;
        }
        SetPositionTo(&snake_map->clientSnake.snake.headPos, &newPosition);

        snake_map->map[relative.x][relative.y] = DirToChar(&snake_map->clientSnake.snake.direction);
        snake_map->clientSnake.snake.time++;

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
        SetBackgroundColor(snake_map->snake.bodyColor);
        AbsolutePosition(&absolute,previousPosition.x,previousPosition.y,&snake_map->gameSize);
        DrawDot(absolute.x,absolute.y);
        SetPositionTo(&currentPosition, &snake_map->snake.tailHelp);
        AbsolutePosition(&absolute, currentPosition.x,currentPosition.y, &snake_map->gameSize);
        ResetTextEffect();
        DrawDot(absolute.x,absolute.y);

        AbsolutePosition(&absolute, snake_map->snake.headPos.x, snake_map->snake.headPos.y, &snake_map->gameSize);
        SetBackgroundColor(snake_map->snake.headColor);
        DrawDot(absolute.x,absolute.y);

        DrawSnakeStats(&snake_map->snake);
    }
}

void UpdateApple(snake_map_t *snake_map) {
    position_t relative;
    SetPositionXY(&snake_map->clientSnake.apple,rand()%GetSize(&snake_map->clientSnake.gameSize), rand()%GetSize(&snake_map->clientSnake.gameSize));
    RelativePosition(&relative,snake_map->clientSnake.apple.x,snake_map->clientSnake.apple.y, &snake_map->clientSnake.gameSize);
    while (snake_map->map[relative.x][relative.y] != ' ') {
        SetPositionXY(&snake_map->clientSnake.apple,rand()%GetSize(&snake_map->clientSnake.gameSize), rand()%GetSize(&snake_map->clientSnake.gameSize));
        RelativePosition(&relative,snake_map->clientSnake.apple.x,snake_map->clientSnake.apple.y, &snake_map->clientSnake.gameSize);
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

void ToString(client_snake_t const *snake_map, char * data) {
    int alive  = snake_map->snake.isAlive;
    int active = snake_map->snake.isActive;
    sprintf(data,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
        snake_map->gameSize,
        snake_map->snake.playerNumber,
        snake_map->snake.headColor, snake_map->snake.bodyColor,
        snake_map->snake.headPos.x, snake_map->snake.headPos.y,
        snake_map->snake.direction.x, snake_map->snake.direction.y,
        snake_map->snake.tailPos.x, snake_map->snake.tailPos.y,
        snake_map->snake.tailHelp.x, snake_map->snake.tailHelp.y,
        snake_map->snake.size, snake_map->snake.time,
        snake_map->apple.x, snake_map->apple.y,
        alive,active);
}

int ReadString(char const *data, client_snake_t * snake) {
    int alive, active;
    bool success = sscanf(data,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
        &snake->gameSize,
        &snake->snake.playerNumber,
        &snake->snake.headColor, &snake->snake.bodyColor,
        &snake->snake.headPos.x, &snake->snake.headPos.y,
        &snake->snake.direction.x, &snake->snake.direction.y,
        &snake->snake.tailPos.x, &snake->snake.tailPos.y,
        &snake->snake.tailHelp.x, &snake->snake.tailHelp.y,
        &snake->snake.size, &snake->snake.time,
        &snake->apple.x, &snake->apple.y,
        &alive, &active) == 18;
    snake->snake.isAlive = alive == 1;
    snake->snake.isActive = active == 1;
    return success;
}

void GiveServerString(client_snake_t const *snake_map, char *data) {
    int alive  = snake_map->snake.isAlive;
    int active = snake_map->snake.isActive;
    sprintf(data,"%d;%d;%d;%d;%d;%d;%d;",
        snake_map->gameSize,snake_map->gameType,
        snake_map->snake.playerNumber,
        snake_map->snake.direction.x, snake_map->snake.direction.y,
        alive, active
        );
}

int ServerReadString(char const *data, client_snake_t *snake) {
    int alive, active;
    bool success = sscanf(data,"%d;%d;%d;%d;%d;%d;%d;",
        &snake->gameSize,&snake->gameType,
        &snake->snake.playerNumber,
        &snake->snake.direction.x, &snake->snake.direction.y,
        &alive, &active) == 7;
    snake->snake.isAlive = alive == 1;
    snake->snake.isActive = active == 1;
    return success;
}

