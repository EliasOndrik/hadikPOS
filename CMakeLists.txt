cmake_minimum_required(VERSION 4.1)
project(hadikPOS C)

set(CMAKE_C_STANDARD 11)

add_executable(hadikPOS main.c
        console.c
        console.h
        client.c
        client.h
        server.c
        server.h
        snake.c
        snake.h
        menu.c
        menu.h)

if (WIN32)
    target_link_libraries(hadikPOS ws2_32)
endif ()

