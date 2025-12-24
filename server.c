#include "server.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <asm-generic/socket.h>
#include <netinet/in.h>
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

    int server_fd = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Chyba pri vytvarani socketu");
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
        return -2;
    }
    printf("Socket naviazany na port: %d\n", PORT);
    if (listen(server_fd, 5) < 0) {
        perror("Listen zlyhal");
        close(server_fd);
        return -3;
    }
    printf("Server pocuva ...\n");

    int client_fd;
    struct sockaddr_in client_address;
    socklen_t client_len;
    client_len = sizeof(client_address);
    client_fd = (int)accept(server_fd, (struct sockaddr *)&client_address, &client_len);
    if (client_fd < 0) {
        perror("Accept zlyhal");
        close(server_fd);
        return -4;
    }
    printf("Klient pripojeny: %s:%d\n",inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port));

    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(client_fd, buffer, BUFFER_SIZE-1, 0);
        if (bytes_read <= 0) {
            printf("Klient sa odpojil\n");
            break;
        }
        printf("Prijata sprava: %s\n", buffer);

        if (strncmp(buffer, "quit",4) == 0) {
            printf("Ukoncujem spojenie\n");
            break;
        }
        send(client_fd, buffer, (int)strlen(buffer), 0);
    }
    close(client_fd);
    close(server_fd);
    printf("Server je ukonceny\n");
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
