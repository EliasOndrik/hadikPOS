#include "snake.h"

void SetSnakeMap(snake_map_t *snake_map) {
    ResetTextEffect();
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j = 0; j < MAX_HEIGHT; j++) {
            snake_map->snakeMap[i][j] = ' ';
            DrawDot(16 + i,1 + j);
        }
    }

}

void PlaceSnakeOnMap(snake_map_t *snake_map) {
    snake_map->snake.bodyColor = 2;
    snake_map->snake.headColor = 118;
    snake_map->snake.size = 3;
    snake_map->snake.playerNumber = 1;
    SetPositionXY(&snake_map->snake.headPos, 3,14);
    SetPositionXY(&snake_map->snake.tailPos,1,14);
    SetBackgroundColor(snake_map->snake.bodyColor);
    for (int i = 1; i <4;i++) {
        snake_map->snakeMap[i][14] = '>';
        DrawDot(16 + i,1 + 14);
    }
    ResetTextEffect();
    SetPositionXY(&snake_map->snake.direction,1,0);
    DrawApple(snake_map);

}

void Update(snake_map_t *snake_map) {
    position_t newPosition;
    SetPositionXY(&newPosition,snake_map->snake.headPos.x + snake_map->snake.direction.x,snake_map->snake.headPos.y + snake_map->snake.direction.y);
    position_t currentPosition;
    SetPositionTo(&currentPosition, &snake_map->snake.headPos);
    snake_map->snakeMap[currentPosition.x][currentPosition.y] = DirToChar(&snake_map->snake.direction);

    if (PositionEquals(&newPosition, &snake_map->apple)) {
        snake_map->snake.size++;
        DrawApple(snake_map);
    } else {
        SetPositionTo(&currentPosition, &snake_map->snake.tailPos);
        char ch = snake_map->snakeMap[snake_map->snake.tailPos.x][snake_map->snake.tailPos.y];
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
        DrawDot(16 + snake_map->snake.tailPos.x,1 + snake_map->snake.tailPos.y);
        snake_map->snakeMap[snake_map->snake.tailPos.x][snake_map->snake.tailPos.y] = ' ';
        SetPositionTo(&snake_map->snake.tailPos,&currentPosition);
    }


    if (newPosition.x < 0 || newPosition.x >= MAX_WIDTH || newPosition.y <0 || newPosition.y >= MAX_HEIGHT) {
        return;
    }
    SetPositionTo(&snake_map->snake.headPos, &newPosition);
    SetBackgroundColor(snake_map->snake.bodyColor);
    DrawDot(16 + newPosition.x,1 + newPosition.y);

    snake_map->snakeMap[newPosition.x][newPosition.y] = DirToChar(&snake_map->snake.direction);

    ResetTextEffect();
    MoveTo(3,2);
    printf("%lld", time(NULL));
}

void DrawApple(snake_map_t *snake_map) {
    SetPositionXY(&snake_map->apple,rand()%MAX_WIDTH, rand()%MAX_HEIGHT);
    while (snake_map->snakeMap[snake_map->apple.x][snake_map->apple.y] != ' ') {
        SetPositionXY(&snake_map->apple,rand()%MAX_WIDTH, rand()%MAX_HEIGHT);
    }
    snake_map->snakeMap[snake_map->apple.x][snake_map->apple.y] = 'a';
    SetBackgroundColor(1);
    DrawDot(16+snake_map->apple.x, 1+snake_map->apple.y);
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
