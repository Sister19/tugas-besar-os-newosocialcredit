#ifndef _SCREEN_H
#define _SCREEN_H

#include "interrupt.h"
// Write endline
#define endl printString("\r\n")
// Write new space
#define sp printString(" ")

int cursorX, cursorY;

// Get the cursor position
void getCursor();
// Set the cursor position
void setCursor(int x, int y);
// Set screen video mode
void setVideoMode(int mode);
// Clear the screen
void clearScreen();
// Print a character into the screen
void printChar(char c);
// Print a string into the screen
void printString(char *string);
// Delete a character from the current cursor
void deleteChar();

#endif