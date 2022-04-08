#include "keyboard.h"
#include "screen.h"

void readKey(int* scancode, int* key) {
    int AX = intr(INT_KEYBOARD, AX_KEYBOARD_READ, 0, 0, 0);
    *scancode = REG_H(AX);
    *key = REG_L(AX);
}

char history[MAX_HIST][MAX_INPUT];
static int hist_length = 0;
void changeHistory(char* buffer, bool down, int *len, int *hidx, int *index) {
    int x, y;
    int i = 0;
    *hidx += (down ? 1 : -1);
    // clear text
    while (buffer[i] != nullt) {
        buffer[i] = nullt;
        printChar(KEY_BKSP);
        i++;
    }
    strcpy(buffer, history[*hidx]);
    x = -1; y = -1; // we just wanna get the startcursor
    startCursor(&x, &y);
    setCursor(x, y);
    printString(buffer);
    *len = strlen(buffer);
    *index = *len;
}

void readString(char *buffer)
{
    int i = 0;
    int index = 0, len = 0, hidx = hist_length;
    int input, scancode;
    int x, y;
    getCursor(&x, &y);
    startCursor(&x, &y);
    clear(buffer, MAX_INPUT);
    while (true)
    {
        // get interrupt for 0x16 (used to scan keyboard input)
        readKey(&scancode, &input);
        // if l/r arrow key, we match the cursor
        if (scancode == SC_LARROW && index > 0) {
            --index;
            backwardCursor();
        }
        else if (scancode == SC_RARROW && index < len) {
            ++index;
            forwardCursor();
        }
        // if there's history, we cycle it
        else if (scancode == SC_UARROW && hidx > 0) {
            changeHistory(buffer, false, &len, &hidx, &index);
        }
        else if (scancode == SC_DARROW && hidx < hist_length - 1) {
            changeHistory(buffer, true, &len, &hidx, &index);
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
            if (len > 0){
                if (hist_length == MAX_HIST) {
                    i = 0;
                    while (i < MAX_HIST - 1) {
                        strcpy(history[i], history[i+1]);
                        ++i;
                    }
                    strcpy(history[i], buffer);
                } else {
                    strcpy(history[hist_length++], buffer);
                }
            }
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

