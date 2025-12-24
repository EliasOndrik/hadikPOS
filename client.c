#include "client.h"
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
        return -2;
    }
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Pripojenie zlyhalo\n");
        close(client_fd);
        return -3;
    }
    printf("Pripojeny k serveru\n");
    printf("Zadaj spravu(quit = koniec): \n");
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("> ");
        fflush(stdout);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        send(client_fd, buffer, (int)strlen(buffer), 0);
        if (strncmp(buffer, "quit", 4) == 0) {
            printf("Ukoncujem\n");
            break;
        }
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read < 0) {
            printf("Server sa odpojil\n");
            break;
        }
        printf("Echo zo servera : %s\n",buffer);
    }
    close(client_fd);
    printf("Klient ukonceny\n");
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}