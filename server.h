#ifndef HADIKPOS_SERVER_H
#define HADIKPOS_SERVER_H

#endif //HADIKPOS_SERVER_H

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

#include "snake.h"