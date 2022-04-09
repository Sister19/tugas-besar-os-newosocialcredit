#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "../library/std_lib.h"
#include "../includes/interrupt.h"
#include "../includes/constant.h"

// Utils
#define IS_PRINTABLE(input) (input >= 0x20 && input <= 0x7e)

extern int getKeyPress();
void readString(char* buffer);
void readKey(int* scancode, int* key);
bool bufferKey(int* scancode, int* key);

#endif