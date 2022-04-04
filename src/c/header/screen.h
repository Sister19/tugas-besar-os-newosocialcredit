#ifndef _SCREEN_H
#define _SCREEN_H

#include "interrupt.h"
// Write endline
#define endl printChar('\n')
// Write new space
#define sp printChar(' ')

// Color
#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN 0x6
#define COLOR_GRAY 0x7
#define COLOR_DARK_GRAY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW 0xE
#define COLOR_WHITE 0xF

int cursorX, cursorY;

// Get the cursor position
void getCursor();
// Set the cursor position
void setCursor(int x, int y);
// Forward the cursor
void forwardCursor();
// Backward the cursor
void backwardCursor();
// Clear the screen
void clearScreen();
// Print a character into the screen
void printChar(char c);
void printCharColored(char c, char color);
// Print a string into the screen
void printString(char *string);
// Print a string into the screen with color
void printStringColored(char *c, int warna);
// Delete a character from the current cursor
void deleteChar();
void printTitle();

#endif