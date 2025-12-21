#ifndef HADIKPOS_CONSOLE_H
#define HADIKPOS_CONSOLE_H

#endif //HADIKPOS_CONSOLE_H
#include <stdio.h>
#include <stdbool.h>

void MoveTo(int x, int y);
void DrawRect(int x, int y, int width, int height);
void DrawDot(int x, int y);
void SetTextColor(int color);
void SetBackgroundColor(int color);
void WriteLine(char* text);
void Write(char* text);
void SetBold(bool bold);
void SetBlinking(bool blinking);
void SetCursorVisibility(bool visible);
void ResetTextEffect();
void ClearScreen();

