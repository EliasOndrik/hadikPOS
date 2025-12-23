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
#define X_OFFSET ((CONSOLE_WIDTH-MAX_WIDTH)/2)
#define Y_OFFSET ((CONSOLE_HEIGHT-MAX_HEIGHT)/2)


typedef enum GameSize {
    SMALL,
    MEDIUM,
    BIG
} game_size_t;

typedef enum GameType {
    LOOP,
    EDGE
}game_type_t;

typedef struct Position {
    int x;
    int y;
} position_t;

typedef struct Snake {
    position_t tailPos;
    position_t headPos;
    position_t direction;
    int bodyColor;
    int headColor;
    int size;
    int playerNumber;
    bool isAlive;
    bool isActive;
} snake_t;

typedef struct SnakeMap {
    char map[MAX_WIDTH][MAX_HEIGHT];
    snake_t snake;
    position_t apple;
    game_type_t gameType;
    game_size_t gameSize;
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
void LoopPosition(position_t * position, game_size_t const * gameSize);
int GetSize(game_size_t const * gameSize);
int AbsoluteOffestX(game_size_t const * gameSize);
int AbsoluteOffsetY(game_size_t const * gameSize);
void AbsolutePosition(position_t *position, int x, int y,game_size_t const * gameSize);
int RelativeOffsetX(game_size_t const * gameSize);
int RelativeOffsetY(game_size_t const * gameSize);
void RelativePosition(position_t *position, int x, int y,game_size_t const * gameSize);



