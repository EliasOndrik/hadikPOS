#ifndef HADIKPOS_CONSOLE_H
#define HADIKPOS_CONSOLE_H

#endif //HADIKPOS_CONSOLE_H
#include <stdio.h>
#include <stdbool.h>
#define CONSOLE_WIDTH 60
#define CONSOLE_HEIGHT 30

void move_to(int x, int y);
void draw_rect(int x, int y, int width, int height);
void draw_rect_full(int x, int y, int width, int height);
void draw_dot(int x, int y);
void text_color(int color);
void set_background_color(int color);
void write_line(char* text);
void print_text(char* text);
void set_bold(bool bold);
void set_blinking(bool blinking);
void set_cursor_visibility(bool visible);
void reset_text_effect();
void clear_screen();

