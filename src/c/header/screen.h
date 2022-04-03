#ifndef _SCREEN_H
#define _SCREEN_H

#include "interrupt.h"
#include "constant.h"

// Write endline
#define endl printChar('\n')
// Write new space
#define sp printChar(' ')

int cursorX, cursorY;

extern int getCursorPos();

// Get the cursor position
void getCursor();
// Set the cursor position
void setCursor(int x, int y);
// Forward the cursor
void forwardCursor();
// Backward the cursor
void backwardCursor();
// Set screen video mode
void setVideoMode(int mode);
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