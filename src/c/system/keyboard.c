#include "keyboard.h"
#include "screen.h"

void readKey(char* scancode, char* key) {
    int AX = intr(INT_KEYBOARD, AX_KEYBOARD_READ, 0, 0, 0);
    *scancode = REG_H(AX);
    *key = REG_L(AX);
}

void readString(char *buffer)
{
    int i = 0;
    int index = 0, len = 0;
    int input, scancode;
    int x, y;
    getCursor(&x, &y);
    startCursor(&x, &y);
    clear(buffer, MAX_INPUT);
    while (true)
    {
        // get interrupt for 0x16 (used to scan keyboard input)
        readKey(&scancode, &input);
        //printString(itoa(input)); sp; printString(itoa(scancode)); endl;
        // if l/r arrow key, we match the cursor
        if (scancode == SC_LARROW && index > 0) {
            --index;
            backwardCursor();
        }
        else if (scancode == SC_RARROW && index < len) {
            ++index;
            forwardCursor();
        }
        // if CTRL+C, cancel input
        else if (scancode == SC_CTRL && input == 0x3) {
            // cancel input
            buffer[0] = nullt;
            return;
        }
        // If input is either CR or LF (enter), finish reading string
        else if (input == KEY_CR || input == KEY_LF)
        {
            buffer[len] = nullt;
            return;
        }
        // delete input if input is backspace (0x8) and there's any buffer left
        else if (input == KEY_BKSP && index > 0)
        {
            for (i = --index; i < len - 1; i++)
            {
                buffer[i] = buffer[i + 1];
            }
            buffer[--len] = nullt;
            backwardCursor(); // backward cursor and save the pos
            getCursor(&x, &y);
            endCursor(&x, &y);
            // go to first cursor and print buffer
            x = -1; y = -1; // we just wanna get the startcursor
            startCursor(&x, &y);
            setCursor(x, y);
            printString(buffer);
            // forward to get the deleted char and backspace it
            forwardCursor();
            printChar(KEY_BKSP);
            // restore cursor
            x = -1; y = -1; // we just wanna get the end cursor
            endCursor(&x, &y);
            setCursor(x, y);
        }
        // get the input if it's a printable character
        else if (IS_PRINTABLE(input) && len < MAX_INPUT - 1) // left for nullt
        {
            for (i = len++; i > index; i--) {
                buffer[i] = buffer[i - 1];
            }
            buffer[index++] = input;
            forwardCursor(); // forward cursor and save the pos
            getCursor(&x, &y);
            endCursor(&x, &y);
            // go to first cursor and print buffer
            x = -1; y = -1; // we just wanna get the startcursor
            startCursor(&x, &y);
            setCursor(x, y);
            printString(buffer);
            // restore the pos
            x = -1; y = -1; // we just wanna get the end cursor
            endCursor(&x, &y);
            setCursor(x, y);
        }
        // otherwise we ignore input
    }
}

