#ifndef HADIKPOS_CLIENT_H
#define HADIKPOS_CLIENT_H

#endif //HADIKPOS_CLIENT_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <conio.h>
#include <pthread.h>
#include "menu.h"

typedef struct ThreadArgs {
    int client_fd;
    client_snake_t* snake;
    menu_t* menu;
    pthread_mutex_t* mutex;
    pthread_cond_t* sendData;
    int isRunning;
    bool canUpdate;
} thread_args_t;

void * InputThread(void * arg);
void * DrawThread(void* arg);