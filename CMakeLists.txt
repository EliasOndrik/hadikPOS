cmake_minimum_required(VERSION 4.1)
project(hadikPOS C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(hadikPOS main.c
        console.c
        console.h
        snake.c
        snake.h
        menu.c
        menu.h)

if (WIN32)
    target_link_libraries(hadikPOS ws2_32)
endif ()

add_executable(hadikServer server.h
        server.c
        snake.h
        snake.c
        console.h
        console.c
)

if (WIN32)
    target_link_libraries(hadikServer ws2_32)
endif ()

add_executable(hadikClient client.h
        client.c
        snake.h
        snake.c
        console.h
        console.c
        menu.c
        menu.h
)

if (WIN32)
    target_link_libraries(hadikClient ws2_32)
endif ()
