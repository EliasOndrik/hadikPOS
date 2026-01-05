#include "console.h"

void move_to(int x, int y) {
    printf("\x1B[%d;%dH",y,x);
    //fputs("\x1B[10;10H", stdout);
}

void draw_rect(int x, int y, int width, int height) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            if (i >x && i < x + width -1 && j > y && j < y+height-1) {
                continue;
            }
            draw_dot(i,j);
        }
    }
}

void draw_rect_full(int x, int y, int width, int height) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            draw_dot(i,j);
        }
    }
}

void draw_dot(int x, int y) {
    move_to(x*2 + 1,y + 1);
    print_text("  ");
}

void text_color(int color) {
    if (color < 0) {
        color *= -1;
    }
    color = color % 256;
    printf("\x1B[38;5;%dm", color);
}

void set_background_color(int color) {
    if (color < 0) {
        color *= -1;
    }
    color = color % 256;
    printf("\x1B[48;5;%dm", color);
}

void write_line(char *text) {
    printf("%s\n",text);
}

void print_text(char *text) {
    printf("%s",text);
}

void set_bold(bool bold) {
    if (bold) {
        printf("\x1B[1m");
    } else {
        printf("\x1B[22m");
    }
}

void set_blinking(bool blinking) {
    if (blinking) {
        printf("\x1B[5m");
    } else {
        printf("\x1B[25m");
    }
}

void set_cursor_visibility(bool visible) {
    if (visible) {
        printf("\x1B[?25h");
    } else {
        printf("\x1B[?25l");
    }
}

void reset_text_effect() {
    printf("\x1B[0m");
}

void clear_screen() {
    move_to(0,0);
    printf("\x1B[J");
}

