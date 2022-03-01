#include "header/keyboard.h"
#include "header/constant.h"
#include "header/screen.h"



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
        if (input == 0xD || input == 0xA)
        {
            buffer[index] = nullt;
            return;
        }
        // delete input if input is backspace (0x8) and there's any buffer left
        else if (input == 0x8 && index > 0)
        {
            buffer[index--] = nullt;
            deleteChar();
        }
        // get the input if it's a printable character
        else if (input >= 0x20 && input <= 0x7e && index < length)
        {
            buffer[index++] = input;
            // display character
            printChar(input);
        }
        // otherwise we ignore input
    }
}

