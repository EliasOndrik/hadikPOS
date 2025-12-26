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
#include "menu.h"