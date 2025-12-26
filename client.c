#include "client.h"

#define PORT 14589
#define BUFFER_SIZE 1024
int main() {
#ifdef _WIN32
    struct WSAData wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
    int client_fd = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Vytvorenie socketu zlyhalo\n");
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }
    printf("Socket vytvoreny\n");
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) < 0) {
        perror("Neplatna adresa\n");
        close(client_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -2;
    }
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Pripojenie zlyhalo\n");
        close(client_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -3;
    }
    printf("Pripojeny k serveru\n");
    srand(time(NULL));
    ClearScreen();
    SetCursorVisibility(false);
    client_snake_t snake;
    DrawMenu();
    menu_t menu;
    CreateButtons(&menu);
    UpdateMenu(&menu,' ');

    char buffer[BUFFER_SIZE] = {0};
    int bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read < 0) {
        printf("Server sa odpojil\n");
        close(client_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -4;
    }
    ReadString(buffer, &snake);
    snake.snake.isActive = false;
    bool alive  = snake.snake.isAlive;
    bool active = snake.snake.isActive;
    while (1) {
        if (kbhit()) {
            char key = (char)getch();
            if (key == 'w' && snake.snake.direction.y != 1) {
                SetPositionXY(&snake.snake.direction,0,-1);
            }
            if (key == 's' && snake.snake.direction.y != -1) {
                SetPositionXY(&snake.snake.direction,0,+1);
            }
            if (key == 'a' && snake.snake.direction.x != 1) {
                SetPositionXY(&snake.snake.direction,-1,0);
            }
            if (key == 'd' && snake.snake.direction.x != -1) {
                SetPositionXY(&snake.snake.direction,+1,0);
            }
            int state = UpdateMenu(&menu,key);
            if (state == -1) {
                break;
            }
            if (state == 0) {
                snake.snake.isActive = true;
            } else {
                snake.snake.isActive = false;
                snake.gameSize = menu.gameSize;
                snake.gameType = menu.gameType;
                snake.snake.isAlive = false;
            }
            alive  = snake.snake.isAlive;
            active = snake.snake.isActive;
        }
        memset(buffer, 0, BUFFER_SIZE);
        GiveServerString(&snake, buffer);
        send(client_fd, buffer, (int)strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read < 0) {
            printf("Server sa odpojil\n");
            break;
        }
        ReadString(buffer, &snake);

        if (active) {
            if (!alive) {
                DrawSnakeMap(&snake);
                DrawSnakeOnMap(&snake);
            }
            Draw(&snake);
            DrawApple(&snake);
        }
        active = snake.snake.isActive;
        alive  = snake.snake.isAlive;
        usleep(100*1000);
    }
    close(client_fd);
    ResetTextEffect();
    SetCursorVisibility(true);
    ClearScreen();
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}