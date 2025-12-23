#include "snake.h"

void SetSnakeMap(snake_map_t *snake_map) {
    ResetTextEffect();
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j = 0; j < MAX_HEIGHT; j++) {
            snake_map->map[i][j] = ' ';
            DrawDot(X_OFFSET + i,Y_OFFSET + j);
        }
    }
}

void PlaceSnakeOnMap(snake_map_t *snake_map) {
    snake_map->gameType = LOOP;
    snake_map->gameSize = BIG;
    snake_map->snake.bodyColor = 2;
    snake_map->snake.headColor = 118;
    snake_map->snake.size = 3;
    snake_map->snake.playerNumber = 1;
    SetPositionXY(&snake_map->snake.headPos, 3,GetSize(&snake_map->gameSize)/2);
    SetPositionXY(&snake_map->snake.tailPos,1,GetSize(&snake_map->gameSize)/2);
    SetBackgroundColor(snake_map->snake.bodyColor);
    for (int i = 1; i <4;i++) {
        position_t relative;
        RelativePosition(&relative, i,(GetSize(&snake_map->gameSize)/2), &snake_map->gameSize);
        position_t absolute;
        AbsolutePosition(&absolute, i,(GetSize(&snake_map->gameSize)/2), &snake_map->gameSize);
        snake_map->map[relative.x][relative.y] = '>';
        DrawDot(absolute.x,absolute.y);
    }
    snake_map->snake.isAlive = true;
    snake_map->snake.isActive = true;
    ResetTextEffect();
    SetPositionXY(&snake_map->snake.direction,1,0);
    DrawApple(snake_map);

}

void Update(snake_map_t *snake_map) {
    if (snake_map->snake.isActive) {
        position_t relative;
        position_t absolute;

        position_t newPosition;
        SetPositionXY(&newPosition,snake_map->snake.headPos.x + snake_map->snake.direction.x,snake_map->snake.headPos.y + snake_map->snake.direction.y);
        position_t currentPosition;
        SetPositionTo(&currentPosition, &snake_map->snake.headPos);
        RelativePosition(&relative, currentPosition.x, currentPosition.y, &snake_map->gameSize);
        snake_map->map[relative.x][relative.y] = DirToChar(&snake_map->snake.direction);
        SetBackgroundColor(snake_map->snake.bodyColor);
        AbsolutePosition(&absolute,currentPosition.x,currentPosition.y,&snake_map->gameSize);
        DrawDot(absolute.x,absolute.y);

        if (PositionEquals(&newPosition, &snake_map->apple)) {
            snake_map->snake.size++;
            DrawApple(snake_map);
        } else {
            SetPositionTo(&currentPosition, &snake_map->snake.tailPos);
            RelativePosition(&relative, currentPosition.x,currentPosition.y, &snake_map->gameSize);
            AbsolutePosition(&absolute, currentPosition.x,currentPosition.y, &snake_map->gameSize);
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
            ResetTextEffect();
            DrawDot(absolute.x,absolute.y);
            snake_map->map[relative.x][relative.y] = ' ';
            if (snake_map->gameType == LOOP) {
                LoopPosition(&currentPosition,&snake_map->gameSize);
            }
            SetPositionTo(&snake_map->snake.tailPos,&currentPosition);
        }

        switch (snake_map->gameType) {
            case LOOP :
                LoopPosition(&newPosition,&snake_map->gameSize);
                break ;
            case EDGE :
                if ((newPosition.x < 0 || newPosition.x >= GetSize(&snake_map->gameSize) || newPosition.y <0 || newPosition.y >= GetSize(&snake_map->gameSize))) {
                    snake_map->snake.isAlive = false;
                    return;
                }
                break ;
            default:
                break;
        }

        RelativePosition(&relative, newPosition.x,newPosition.y, &snake_map->gameSize);
        AbsolutePosition(&absolute, newPosition.x, newPosition.y, &snake_map->gameSize);
        if (!(snake_map->map[relative.x][relative.y] == ' ' || snake_map->map[relative.x][relative.y] == 'a')) {
            snake_map->snake.isAlive = false;
            return;
        }
        SetPositionTo(&snake_map->snake.headPos, &newPosition);
        SetBackgroundColor(snake_map->snake.headColor);
        DrawDot(absolute.x,absolute.y);

        snake_map->map[relative.x][relative.y] = DirToChar(&snake_map->snake.direction);

        ResetTextEffect();
        MoveTo(3,2);
        printf("%d  ", snake_map->snake.size);
    }
}

void DrawApple(snake_map_t *snake_map) {
    position_t relative;
    position_t absolute;
    SetPositionXY(&snake_map->apple,rand()%GetSize(&snake_map->gameSize), rand()%GetSize(&snake_map->gameSize));
    RelativePosition(&relative,snake_map->apple.x,snake_map->apple.y, &snake_map->gameSize);
    while (snake_map->map[relative.x][relative.y] != ' ') {
        SetPositionXY(&snake_map->apple,rand()%GetSize(&snake_map->gameSize), rand()%GetSize(&snake_map->gameSize));
        RelativePosition(&relative,snake_map->apple.x,snake_map->apple.y, &snake_map->gameSize);
    }
    snake_map->map[relative.x][relative.y] = 'a';
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
