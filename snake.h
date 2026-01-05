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

void initialize_map(snake_map_t *snakeMap);
void initialize_snake(client_snake_t *client);
void set_snake_map(snake_map_t* snakeMap);
void draw_snake_map(client_snake_t* client);
void place_snake_on_map(snake_map_t* snakeMap, int playerIndex);
void draw_snake_on_map(client_snake_t* client);
void update(snake_map_t* snakeMap, int playerIndex);
void draw(client_snake_t* client);
void update_apple(snake_map_t *snakeMap, int playerIndex);
void draw_apple(client_snake_t *client);
void set_position_to(position_t *position, position_t const *newPosition);
void set_position_xy(position_t* position, int x, int y);
bool position_equals(position_t const *position, position_t const *newPosition);
char dir_to_char(position_t const *direction);
position_t char_to_dir(char const *direction);
void loop_position(position_t * position, game_size_t const * gameSize);
int get_size(game_size_t const * gameSize);
int absolute_offest_x(game_size_t const * gameSize);
int absolute_offset_y(game_size_t const * gameSize);
void absolute_position(position_t *position, int x, int y,game_size_t const * gameSize);
int relative_offset_x(game_size_t const * gameSize);
int relative_offset_y(game_size_t const * gameSize);
void relative_position(position_t *position, int x, int y,game_size_t const * gameSize);
void draw_snake_stats(snake_t * snake);
void to_string(client_snake_t const* client, char * data);
int read_string(char const* data, client_snake_t * client);
void give_server_string(client_snake_t const* client, char * data);
int server_read_string(char const* data, client_snake_t * client);
int get_snake_head_color(int playerNumber);
int get_snake_body_color(int playerNumber);




