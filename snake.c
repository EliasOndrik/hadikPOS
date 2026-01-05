#include "snake.h"

void InitializeMap(snake_map_t *snakeMap) {
    for (int i = 0; i <MAX_PLAYERS; i++) {
        InitializeSnake(&snakeMap->clientSnake[i]);
    }
    snakeMap->maxPlayerCount = 1;
}

void InitializeSnake(client_snake_t *client) {
    client->snake.isActive = false;
    client->snake.isAlive = false;
    client->snake.playerNumber = -1;
    client->snake.time = 0;
    SetPositionXY(&client->snake.direction, 0,0);
    SetPositionXY(&client->snake.headPos, -1,-1);
    SetPositionXY(&client->snake.tailPos, -1,-1);
    SetPositionXY(&client->snake.tailHelp, -1,-1);
    client->snake.size = 0;
    client->gameSize = BIG;
    client->gameType = LOOP;
    SetPositionXY(&client->apple, -1,-1);
    client->timeout = 0;

}

void SetSnakeMap(snake_map_t *snakeMap) {
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j = 0; j < MAX_HEIGHT; j++) {
            snakeMap->map[i][j] = ' ';
        }
    }

}

void DrawSnakeMap(client_snake_t *client) {
    ResetTextEffect();
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j = 0; j < MAX_HEIGHT; j++) {
            DrawDot(X_OFFSET + i,Y_OFFSET + j);
        }
    }
    SetBackgroundColor(6);
    if (client->gameSize != BIG) {
        DrawRect(AbsoluteOffestX(&client->gameSize)-1, AbsoluteOffsetY(&client->gameSize)-1,GetSize(&client->gameSize)+2,GetSize(&client->gameSize)+2);
    }
    ResetTextEffect();
}

void PlaceSnakeOnMap(snake_map_t *snakeMap, int playerIndex) {
    snakeMap->clientSnake[playerIndex].snake.size = 3;
    snakeMap->clientSnake[playerIndex].snake.time = 0;
    switch (snakeMap->clientSnake[playerIndex].snake.playerNumber) {
        case 1:
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.headPos, 1,3);
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.tailPos,1,1);
            for (int i = 1; i <4;i++) {
                position_t relative;
                RelativePosition(&relative, 1,i, &snakeMap->clientSnake[playerIndex].gameSize);
                snakeMap->map[relative.x][relative.y] = 'V';
            }
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.direction,0,1);
            break;
        case 2:
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.headPos, 3,GetSize(&snakeMap->clientSnake[playerIndex].gameSize)-2);
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.tailPos,1,GetSize(&snakeMap->clientSnake[playerIndex].gameSize)-2);
            for (int i = 1; i <4;i++) {
                position_t relative;
                RelativePosition(&relative, i,(GetSize(&snakeMap->clientSnake[playerIndex].gameSize))-2, &snakeMap->clientSnake[playerIndex].gameSize);
                snakeMap->map[relative.x][relative.y] = '>';
            }
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.direction,1,0);
            break;
        case 3:
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.headPos, GetSize(&snakeMap->clientSnake[playerIndex].gameSize)-4,1);
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.tailPos,GetSize(&snakeMap->clientSnake[playerIndex].gameSize)-2,1);
            for (int i = 1; i <4;i++) {
                position_t relative;
                RelativePosition(&relative, (GetSize(&snakeMap->clientSnake[playerIndex].gameSize)) -1 -i,1, &snakeMap->clientSnake[playerIndex].gameSize);
                snakeMap->map[relative.x][relative.y] = '<';
            }
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.direction,-1,0);
            break;
        case 4:
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.headPos, GetSize(&snakeMap->clientSnake[playerIndex].gameSize)-2,GetSize(&snakeMap->clientSnake[playerIndex].gameSize)-4);
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.tailPos,GetSize(&snakeMap->clientSnake[playerIndex].gameSize)-2,GetSize(&snakeMap->clientSnake[playerIndex].gameSize)-2);
            for (int i = 1; i <4;i++) {
                position_t relative;
                RelativePosition(&relative, (GetSize(&snakeMap->clientSnake[playerIndex].gameSize))-2,(GetSize(&snakeMap->clientSnake[playerIndex].gameSize))-1-i, &snakeMap->clientSnake[playerIndex].gameSize);
                snakeMap->map[relative.x][relative.y] = 'A';
            }
            SetPositionXY(&snakeMap->clientSnake[playerIndex].snake.direction,0,-1);
            break;
        default: ;
    }
    snakeMap->clientSnake[playerIndex].snake.isAlive = true;
    snakeMap->clientSnake[playerIndex].snake.isActive = true;
    ResetTextEffect();
    UpdateApple(snakeMap, playerIndex);

}

void DrawSnakeOnMap(client_snake_t *client) {
    SetBackgroundColor(GetSnakeBodyColor(client->snake.playerNumber));
    switch (client->snake.playerNumber) {
        case 1:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                AbsolutePosition(&absolute, 1,i, &client->gameSize);
                DrawDot(absolute.x,absolute.y);
            }
            break;
        case 2:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                AbsolutePosition(&absolute, i,(GetSize(&client->gameSize))-2, &client->gameSize);
                DrawDot(absolute.x,absolute.y);
            }
            break;
        case 3:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                AbsolutePosition(&absolute, (GetSize(&client->gameSize)) -1 -i,1, &client->gameSize);
                DrawDot(absolute.x,absolute.y);
            }
            break;
        case 4:
            for (int i = 1; i <4;i++) {
                position_t absolute;
                AbsolutePosition(&absolute, (GetSize(&client->gameSize))-2,(GetSize(&client->gameSize))-1-i, &client->gameSize);
                DrawDot(absolute.x,absolute.y);
            }
            break;
        default: ;
    }
}

void Update(snake_map_t *snakeMap, int playerIndex) {
    if (snakeMap->clientSnake[playerIndex].snake.isActive) {
        position_t relative;
        position_t newPosition;
        SetPositionXY(&newPosition,snakeMap->clientSnake[playerIndex].snake.headPos.x + snakeMap->clientSnake[playerIndex].snake.direction.x,snakeMap->clientSnake[playerIndex].snake.headPos.y + snakeMap->clientSnake[playerIndex].snake.direction.y);
        position_t currentPosition;
        SetPositionTo(&currentPosition, &snakeMap->clientSnake[playerIndex].snake.headPos);
        RelativePosition(&relative, currentPosition.x, currentPosition.y, &snakeMap->clientSnake[playerIndex].gameSize);
        snakeMap->map[relative.x][relative.y] = DirToChar(&snakeMap->clientSnake[playerIndex].snake.direction);
        bool ateApple = false;
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (PositionEquals(&newPosition, &snakeMap->clientSnake[i].apple) && snakeMap->clientSnake[i].snake.isActive) {
                snakeMap->clientSnake[playerIndex].snake.size++;
                UpdateApple(snakeMap, i);
                ateApple = true;
                break;
            }
        }
        if (!ateApple){
            SetPositionTo(&currentPosition, &snakeMap->clientSnake[playerIndex].snake.tailPos);
            SetPositionTo(&snakeMap->clientSnake[playerIndex].snake.tailHelp, &currentPosition);
            RelativePosition(&relative, currentPosition.x,currentPosition.y, &snakeMap->clientSnake[playerIndex].gameSize);
            char ch = snakeMap->map[relative.x][relative.y];
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
            snakeMap->map[relative.x][relative.y] = ' ';
            if (snakeMap->clientSnake[playerIndex].gameType == LOOP) {
                LoopPosition(&currentPosition,&snakeMap->clientSnake[playerIndex].gameSize);
            }
            SetPositionTo(&snakeMap->clientSnake[playerIndex].snake.tailPos,&currentPosition);
        }

        switch (snakeMap->clientSnake[playerIndex].gameType) {
            case LOOP :
                LoopPosition(&newPosition,&snakeMap->clientSnake[playerIndex].gameSize);
                break ;
            case EDGE :
                if ((newPosition.x < 0 || newPosition.x >= GetSize(&snakeMap->clientSnake[playerIndex].gameSize) || newPosition.y <0 || newPosition.y >= GetSize(&snakeMap->clientSnake[playerIndex].gameSize))) {
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
        RelativePosition(&relative, newPosition.x,newPosition.y, &snakeMap->clientSnake[playerIndex].gameSize);
        if (!(snakeMap->map[relative.x][relative.y] == ' ' || snakeMap->map[relative.x][relative.y] == 'a')) {
            snakeMap->clientSnake[playerIndex].snake.isAlive = false;
            return;
        }
        SetPositionTo(&snakeMap->clientSnake[playerIndex].snake.headPos, &newPosition);

        snakeMap->map[relative.x][relative.y] = DirToChar(&snakeMap->clientSnake[playerIndex].snake.direction);
        snakeMap->clientSnake[playerIndex].snake.time++;

    }
}

void Draw(client_snake_t *client) {
    if (client->snake.isActive) {
        position_t absolute;

        position_t previousPosition;
        SetPositionXY(&previousPosition,client->snake.headPos.x - client->snake.direction.x,client->snake.headPos.y - client->snake.direction.y);
        LoopPosition(&previousPosition,&client->gameSize);
        position_t currentPosition;
        SetPositionTo(&currentPosition, &client->snake.headPos);
        SetBackgroundColor(GetSnakeBodyColor(client->snake.playerNumber));
        AbsolutePosition(&absolute,previousPosition.x,previousPosition.y,&client->gameSize);
        DrawDot(absolute.x,absolute.y);
        SetPositionTo(&currentPosition, &client->snake.tailHelp);
        AbsolutePosition(&absolute, currentPosition.x,currentPosition.y, &client->gameSize);
        ResetTextEffect();
        DrawDot(absolute.x,absolute.y);

        AbsolutePosition(&absolute, client->snake.headPos.x, client->snake.headPos.y, &client->gameSize);
        SetBackgroundColor(GetSnakeHeadColor(client->snake.playerNumber));
        DrawDot(absolute.x,absolute.y);

        DrawSnakeStats(&client->snake);
    }
}

void UpdateApple(snake_map_t *snakeMap, int playerIndex) {
    position_t relative;
    SetPositionXY(&snakeMap->clientSnake[playerIndex].apple,rand()%GetSize(&snakeMap->clientSnake[playerIndex].gameSize), rand()%GetSize(&snakeMap->clientSnake[playerIndex].gameSize));
    RelativePosition(&relative,snakeMap->clientSnake[playerIndex].apple.x,snakeMap->clientSnake[playerIndex].apple.y, &snakeMap->clientSnake[playerIndex].gameSize);
    while (snakeMap->map[relative.x][relative.y] != ' ') {
        SetPositionXY(&snakeMap->clientSnake[playerIndex].apple,rand()%GetSize(&snakeMap->clientSnake[playerIndex].gameSize), rand()%GetSize(&snakeMap->clientSnake[playerIndex].gameSize));
        RelativePosition(&relative,snakeMap->clientSnake[playerIndex].apple.x,snakeMap->clientSnake[playerIndex].apple.y, &snakeMap->clientSnake[playerIndex].gameSize);
    }
    snakeMap->map[relative.x][relative.y] = 'a';
}

void DrawApple(client_snake_t *client) {
    position_t absolute;
    SetBackgroundColor(1);
    AbsolutePosition(&absolute, client->apple.x,client->apple.y, &client->gameSize);
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

void ToString(client_snake_t const *client, char * data) {
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

int ReadString(char const *data, client_snake_t * client) {
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

void GiveServerString(client_snake_t const *client, char *data) {
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

int ServerReadString(char const *data, client_snake_t *client) {
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

