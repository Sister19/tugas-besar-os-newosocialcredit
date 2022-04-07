#ifndef _SCREEN_H
#define _SCREEN_H

#include "../includes/interrupt.h"
#include "../includes/constant.h"

// Write endline
#define endl printChar('\n')
// Write new space
#define sp printChar(' ')

extern int getCursorPos();

// Get the cursor position
void getCursor(int* cursorX, int* cursorY);
// Set the cursor position
void setCursor(int x, int y);
void startCursor(int* x, int* y);
void endCursor(int* x, int* y);
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