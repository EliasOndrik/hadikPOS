#include "server.h"

#define PORT 14589
#define BUFFER_SIZE 1024

int main(int argc, char * argv[]) {
    int offset = 0;
    if (argc > 1) {
        offset = atoi(argv[1]);
    }
#ifdef _WIN32
    struct WSAData wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
    srand(time(NULL));
    int serverFd = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        perror("Chyba pri vytvarani socketu");
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }
    printf("Socket bol vytvoreny\n");

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT + offset);

    if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind zlyha");
        close(serverFd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -2;
    }
    printf("Socket naviazany na port: %d\n", PORT + offset);
    if (listen(serverFd, 4) < 0) {
        perror("Listen zlyhal");
        close(serverFd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -3;
    }
    printf("Server pocuva ...\n");

    snake_map_t snakeMap;
    initialize_map(&snakeMap);
    set_snake_map(&snakeMap);

    int clientFd;
    struct sockaddr_in clientAddress;
    socklen_t clientLen;
    clientLen = sizeof(clientAddress);
    clientFd = (int)accept(serverFd, (struct sockaddr *)&clientAddress, &clientLen);
    if (clientFd < 0) {
        perror("Accept zlyhal");
        close(serverFd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -4;
    }
    printf("Klient pripojeny: %s:%d\n",inet_ntoa(clientAddress.sin_addr),ntohs(clientAddress.sin_port));

    place_snake_on_map(&snakeMap,0);

    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    thread_data_t threadData;
    threadData.clientFd = clientFd;
    threadData.snakeMap = &snakeMap;
    threadData.clientCount = 1;
    threadData.mutex = &mutex;
    printf("Vytvaram thread\n");
    pthread_create(&thread, NULL, client_update, &threadData);
    pthread_join(thread, NULL);
    pthread_mutex_destroy(&mutex);
    close(clientFd);
    close(serverFd);
    printf("Server je ukonceny\n");
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

void * client_receive(void *arg) {

    return NULL;
}

void * client_update(void *arg) {
    thread_data_t * data = arg;
    char buffer[BUFFER_SIZE] = {0};
    data->snakeMap->clientSnake[0].snake.playerNumber = 1;
    to_string(&data->snakeMap->clientSnake[0],buffer);
    send(data->clientFd, buffer, (int)strlen(buffer), 0);
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(data->clientFd, buffer, BUFFER_SIZE-1, 0);
        if (bytes_read <= 0) {
            printf("Klient sa odpojil\n");
            data->clientCount--;
            break;
        }
        server_read_string(buffer, &data->snakeMap->clientSnake[0]);

        if (data->snakeMap->clientSnake[0].snake.isActive) {
            if (!data->snakeMap->clientSnake[0].snake.isAlive) {
                set_snake_map(data->snakeMap);
                place_snake_on_map(data->snakeMap,0);
            }
            update(data->snakeMap,0);
        }
        memset(buffer, 0, BUFFER_SIZE);
        to_string(&data->snakeMap->clientSnake[0],buffer);
        send(data->clientFd, buffer, (int)strlen(buffer), 0);
    }
    return NULL;
}
