#include "header/keyboard.h"
#include "header/constant.h"
#include "header/screen.h"

void readKey(char* scancode, char* key) {
    intr(INT_KEYBOARD, AX_KEYBOARD_READ, 0, 0, 0);
    *scancode = REG_H(AX);
    *key = REG_L(AX);
}

void readString(char *buffer)
{
    int index = 0;
    int input;
    while (true)
    {
        // get interrupt for 0x16 (used to scan keyboard input)
        intr(INT_KEYBOARD, AX_KEYBOARD_READ, 0, 0, 0);
        input = REG_L(AX);
        // If input is either CR or LF (enter), finish reading string
        if (input == KEY_CR || input == KEY_LF)
        {
            buffer[index] = nullt;
            return;
        }
        // delete input if input is backspace (0x8) and there's any buffer left
        else if (input == KEY_BKSP && index > 0)
        {
            buffer[index--] = nullt;
            printChar(KEY_BKSP);
        }
        // get the input if it's a printable character
        else if (IS_PRINTABLE(input) && index < MAX_INPUT)
        {
            buffer[index++] = input;
            // display character
            printChar(input);
        }
        // otherwise we ignore input
    }
}

