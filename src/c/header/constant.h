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

#endif