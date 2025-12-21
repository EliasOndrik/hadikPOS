#include "console.h"

void MoveTo(int x, int y) {
    printf("\x1B[%d;%dH",y,x);
}

void DrawRect(int x, int y, int width, int height) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            if (i >x && i < x + width -1 && j > y && j < y+height-1) {
                continue;
            }
            DrawDot(i,j);
        }
    }
}

void DrawDot(int x, int y) {
    MoveTo(x*2 + 1,y);
    Write("  ");
}

void SetTextColor(int color) {
    if (color < 0) {
        color *= -1;
    }
    color = color % 256;
    printf("\x1B[38;5;%dm", color);
}

void SetBackgroundColor(int color) {
    if (color < 0) {
        color *= -1;
    }
    color = color % 256;
    printf("\x1B[48;5;%dm", color);
}

void WriteLine(char *text) {
    printf("%s\n",text);
}

void Write(char *text) {
    printf("%s",text);
}

void SetBold(bool bold) {
    if (bold) {
        printf("\x1B[1m");
    } else {
        printf("\x1B[22m");
    }
}

void SetBlinking(bool blinking) {
    if (blinking) {
        printf("\x1B[5m");
    } else {
        printf("\x1B[25m");
    }
}

void SetCursorVisibility(bool visible) {
    if (visible) {
        printf("\x1B[?25h");
    } else {
        printf("\x1B[?25l");
    }
}

void ResetTextEffect() {
    printf("\x1B[0m");
}

void ClearScreen() {
    MoveTo(0,0);
    printf("\x1B[J");
}

