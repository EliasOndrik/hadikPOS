#ifndef HADIKPOS_SNAKE_H
#define HADIKPOS_SNAKE_H

#endif //HADIKPOS_SNAKE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "console.h"



#define MAX_WIDTH 28
#define MAX_HEIGHT 28
typedef struct Position {
    int x;
    int y;
} position_t;

typedef struct Snake {
    int bodyColor;
    int headColor;
    position_t tailPos;
    position_t headPos;
    position_t direction;
    int size;
    int playerNumber;
} snake_t;

typedef struct SnakeMap {
    char snakeMap[28][28];
    snake_t snake;
    position_t apple;
} snake_map_t;

void SetSnakeMap(snake_map_t* snake_map);
void PlaceSnakeOnMap(snake_map_t* snake_map);
void Update(snake_map_t* snake_map);
void DrawApple(snake_map_t *snake_map);
void SetPositionTo(position_t *position, position_t const *newPosition);
void SetPositionXY(position_t* position, int x, int y);
bool PositionEquals(position_t const *position, position_t const *newPosition);
char DirToChar(position_t const *direction);
position_t CharToDir(char const *direction);




