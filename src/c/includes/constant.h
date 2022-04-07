#ifndef __CONSTANT_H
#define __CONSTANT_H
// Default Constants

// Page number
#define PAGE_NUMBER 0
// Start cursor
#define START_CURSOR 0x0
// End cursor (DH h x DL w, for video mode 3h it will be 0x18 x 0x4f)
#define END_CURSOR 0x184F
// Width of the screen
#define SCREEN_WIDTH 80
// Max cursor position in x coord (rightmost)
#define MAX_CURSOR_X 79
// Height of the screen
#define SCREEN_HEIGHT 25
// Max cursor position in y coord (downmost)
#define MAX_CURSOR_Y 24
// Video mode
#define VIDEO_MODE 0x3
// Null terminated
#define nullt '\0'
// Length of the buffer
#define MAX_INPUT 256
#define MAX_ARGS 8
#define MAX_HIST 7

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

#endif