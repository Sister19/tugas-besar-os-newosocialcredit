#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "std_lib.h"
#include "std_type.h"
#include "interrupt.h"
#include "constant.h"

// Scancodes
#define SC_TAB 0x0F
#define SC_CTRL 0x2E
#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36
#define SC_RARROW 0x4D
#define SC_LARROW 0x4B
#define SC_UARROW 0x48
#define SC_DARROW 0x50

// ASCII Special keycodes
#define KEY_CR 0xD
#define KEY_LF 0xA
#define KEY_BKSP 0x8
#define KEY_SP 0x20

// Utils
#define IS_PRINTABLE(input) (input >= 0x20 && input <= 0x7e)

extern char* input_buf;
extern int firstCursorX, firstCursorY, lastCursorX, lastCursorY;
extern char* history[MAX_HIST];
extern int hist_length;

void readString(char* buffer);
void readKey(char* scancode, char* key);

#endif