#include "server.h"

#define PORT 14589
#define BUFFER_SIZE 1024

int main() {
#ifdef _WIN32
    struct WSAData wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
    srand(time(NULL));
    int server_fd = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Chyba pri vytvarani socketu");
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }
    printf("Socket bol vytvoreny\n");

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind zlyha");
        close(server_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -2;
    }
    printf("Socket naviazany na port: %d\n", PORT);
    if (listen(server_fd, 4) < 0) {
        perror("Listen zlyhal");
        close(server_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -3;
    }
    printf("Server pocuva ...\n");

    snake_map_t snakeMap;
    SetSnakeMap(&snakeMap);

    int client_fd;
    struct sockaddr_in client_address;
    socklen_t client_len;
    client_len = sizeof(client_address);
    client_fd = (int)accept(server_fd, (struct sockaddr *)&client_address, &client_len);
    if (client_fd < 0) {
        perror("Accept zlyhal");
        close(server_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        return -4;
    }
    printf("Klient pripojeny: %s:%d\n",inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port));

    PlaceSnakeOnMap(&snakeMap,0);

    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    thread_data_t threadData;
    threadData.clientFd = client_fd;
    threadData.snakeMap = &snakeMap;
    threadData.clientCount = 1;
    threadData.mutex = &mutex;
    pthread_create(&thread, NULL, ClientUpdate, &threadData);

    pthread_join(thread, NULL);
    pthread_mutex_destroy(&mutex);
    close(client_fd);
    close(server_fd);
    printf("Server je ukonceny\n");
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

void * ClientReceive(void *arg) {

    return NULL;
}

void * ClientUpdate(void *arg) {
    thread_data_t * data = arg;
    char buffer[BUFFER_SIZE] = {0};
    data->snakeMap->clientSnake[0].snake.playerNumber = 1;
    ToString(&data->snakeMap->clientSnake[0],buffer);
    send(data->clientFd, buffer, (int)strlen(buffer), 0);
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(data->clientFd, buffer, BUFFER_SIZE-1, 0);
        if (bytes_read <= 0) {
            printf("Klient sa odpojil\n");
            data->clientCount--;
            break;
        }
        ServerReadString(buffer, &data->snakeMap->clientSnake[0]);
        if (data->snakeMap->clientSnake[0].snake.isActive) {
            if (!data->snakeMap->clientSnake[0].snake.isAlive) {
                SetSnakeMap(data->snakeMap);
                PlaceSnakeOnMap(data->snakeMap,0);
            }
            Update(data->snakeMap,0);
        }
        memset(buffer, 0, BUFFER_SIZE);
        ToString(&data->snakeMap->clientSnake[0],buffer);
        send(data->clientFd, buffer, (int)strlen(buffer), 0);
    }
    return NULL;
}
