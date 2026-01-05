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

    int offset = 1;
    server_addr.sin_port = htons(PORT + offset-1);
    while (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) >= 0) {
        printf("Tento nie");
        close(client_fd);
        client_fd = (int)socket(AF_INET, SOCK_STREAM, 0);
        server_addr.sin_port = htons(PORT + offset);
        offset++;
    }
    char sever[30];
#ifdef _WIN32
    sprintf(sever, "start hadikServer %d",offset -1);
#else
    sprintf(sever, "./hadikServer %d &",offset -1);
#endif
    system(sever);
    sleep(1);

    server_addr.sin_port = htons(PORT + offset-1);
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Pripojenie zlyhalo\n");
    }
    char buffer[BUFFER_SIZE] ={0};
    recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    client_snake_t snake;
    initialize_snake(&snake);
    read_string(buffer, &snake);
    srand(time(NULL));
    clear_screen();
    set_cursor_visibility(false);

    draw_menu();
    menu_t menu;
    initialize_menu(&menu);
    create_buttons(&menu);
    update_menu(&menu,' ');


    snake.snake.isActive = false;

    thread_args_t thread_args;
    pthread_mutex_t mutex;
    pthread_cond_t sendData;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&sendData,NULL);
    thread_args.clientFd = client_fd;
    thread_args.snake = &snake;
    thread_args.menu = &menu;
    thread_args.mutex = &mutex;
    thread_args.sendData = &sendData;
    thread_args.canUpdate = true;
    thread_args.isRunning = 1;
    thread_args.serverAddr = &server_addr;
    pthread_t inputThread, drawThread;
    pthread_create(&inputThread, NULL, input_thread, &thread_args);
    pthread_create(&drawThread, NULL, draw_thread, &thread_args);

    pthread_join(inputThread, NULL);
    pthread_join(drawThread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&sendData);
    close(client_fd);
    reset_text_effect();
    set_cursor_visibility(true);
    clear_screen();
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

void * input_thread(void *arg) {
    thread_args_t * data = arg;
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        pthread_mutex_lock(data->mutex);
        while (data->canUpdate == false) {
            pthread_cond_wait(data->sendData, data->mutex);
        }
        if (kbhit()) {
            char key = (char)getch();
            int state = update_menu(data->menu,key);
            if (state == -1) {
                data->isRunning = -1;
            }
            if (state == 0) {
                if (key == 'w' && data->snake->snake.direction.y != 1) {
                    set_position_xy(&data->snake->snake.direction,0,-1);
                }
                if (key == 's' && data->snake->snake.direction.y != -1) {
                    set_position_xy(&data->snake->snake.direction,0,+1);
                }
                if (key == 'a' && data->snake->snake.direction.x != 1) {
                    set_position_xy(&data->snake->snake.direction,-1,0);
                }
                if (key == 'd' && data->snake->snake.direction.x != -1) {
                    set_position_xy(&data->snake->snake.direction,+1,0);
                }
                data->snake->snake.isActive = true;
            } else if (state == 4) {
                data->snake->snake.isActive = false;
            } else if (state == 7 ) {
                update_menu(data->menu,' ');
                data->snake->snake.isActive = true;

            } else if (state == 6) {
                update_menu(data->menu,' ');
                data->snake->snake.isActive = true;
                close(data->clientFd);
                data->clientFd = (int)socket(AF_INET, SOCK_STREAM, 0);
            } else if (state == 3) {
                update_menu(data->menu,' ');
                data->snake->snake.isActive = true;
            } else {
                data->snake->snake.isActive = false;
                data->snake->gameSize = data->menu->gameSize;
                data->snake->gameType = data->menu->gameType;
                data->snake->timeout = data->menu->time;
                data->snake->snake.isAlive = false;
            }
        }
        data->canUpdate = false;
        pthread_mutex_unlock(data->mutex);
        memset(buffer, 0, BUFFER_SIZE);
        give_server_string(data->snake, buffer);
        send(data->clientFd, buffer, (int)strlen(buffer), 0);
        if (data->isRunning < 0) {
            break;
        }
    }
    return NULL;
}

void * draw_thread(void *arg) {
    thread_args_t * data = arg;
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        bool alive = data->snake->snake.isAlive;
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(data->clientFd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read < 0) {
            printf("Server sa odpojil\n");
            pthread_mutex_lock(data->mutex);
            data->isRunning = -1;
            pthread_mutex_unlock(data->mutex);
            break;
        }
        pthread_mutex_lock(data->mutex);
        read_string(buffer, data->snake);
        pthread_mutex_unlock(data->mutex);

        if (data->snake->snake.isActive) {
            if (!alive) {
                draw_snake_map(data->snake);
                draw_snake_on_map(data->snake);
            }
            draw(data->snake);
            draw_apple(data->snake);
        }

        pthread_mutex_lock(data->mutex);
        data->canUpdate = true;
        pthread_cond_signal(data->sendData);
        pthread_mutex_unlock(data->mutex);
        usleep(100*1000);
        if (data->isRunning < 0) {
            break;
        }
    }
    return NULL;
}
