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
#define MAX_PLAYERS 4


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
    position_t tailHelp;
    int size;
    unsigned int time;
    int playerNumber;
    bool isAlive;
    bool isActive;
} snake_t;

typedef struct ClientSnake {
    snake_t snake;
    position_t apple;
    game_size_t gameSize;
    game_type_t gameType;
    int timeout;
}client_snake_t;

typedef struct SnakeMap {
    char map[MAX_WIDTH][MAX_HEIGHT];
    client_snake_t clientSnake[MAX_PLAYERS];
    int maxPlayerCount;
} snake_map_t;

void SetSnakeMap(snake_map_t* snake_map);
void DrawSnakeMap(client_snake_t* snake_map);
void PlaceSnakeOnMap(snake_map_t* snake_map, int playerIndex);
void DrawSnakeOnMap(client_snake_t* snake_map);
void Update(snake_map_t* snake_map, int playerIndex);
void Draw(client_snake_t* snake_map);
void UpdateApple(snake_map_t *snake_map, int playerIndex);
void DrawApple(client_snake_t *snake_map);
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
void DrawSnakeStats(snake_t * snake);
void ToString(client_snake_t const* snakeMap, char * data);
int ReadString(char const* data, client_snake_t * snake);
void GiveServerString(client_snake_t const* snakeMap, char * data);
int ServerReadString(char const* data, client_snake_t * snake);
int GetSnakeHeadColor(int playerNumber);
int GetSnakeBodyColor(int playerNumber);




