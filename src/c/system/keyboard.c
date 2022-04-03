#include "../header/keyboard.h"
#include "../header/screen.h"
#include "../header/global.h"

void readKey(char* scancode, char* key) {
    int AX = intr(INT_KEYBOARD, AX_KEYBOARD_READ, 0, 0, 0);
    *scancode = REG_H(AX);
    *key = REG_L(AX);
}

int index = 0, len = 0, hidx = 0;
void changeHistory(char* buffer, bool down) {
    int i = 0;
    hidx += (down ? 1 : -1);
    // clear text
    while (buffer[i] != nullt) {
        buffer[i] = nullt;
        printChar(KEY_BKSP);
        i++;
    }
    strcpy(buffer, history[hidx]);
    setCursor(firstCursorX, firstCursorY);
    printString(buffer);
    len = strlen(buffer);
    index = len;
}

void printLines(char *buffer)
{
    int input, scancode, i;
    char* enter = "Press ENTER to next or CTRL+C to exit...";
    bool is_stop = false;
    i = 0;
    while (buffer[i]) {
        if (buffer[i] == '\n') {
            endl;
            printStringColored(enter, 0xF0);
            while(true) {
                readKey(&scancode, &input);
                if (input == KEY_CR || input == KEY_LF) {
                    break;
                } else if (scancode == SC_CTRL && input == 0x3) {
                    is_stop = true;
                    break;
                }
            }
            input = 0;
            while (enter[input++]) {
                printChar(KEY_BKSP);
            }
            if (is_stop)
                return;
        } else {
            printChar(buffer[i]);
        }
        i++;
    }
    endl;
}

void readString(char *buffer)
{
    int i = 0;
    int input, scancode;
    hidx = hist_length;
    index = 0;
    len = 0;
    getCursor();
    firstCursorX = cursorX;
    firstCursorY = cursorY;
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
        // if there's history, we cycle it
        else if (scancode == SC_UARROW && hidx > 0) {
            changeHistory(buffer, false);
        }
        else if (scancode == SC_DARROW && hidx < hist_length - 1) {
            changeHistory(buffer, true);
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
            lastCursorX = cursorX;
            lastCursorY = cursorY;
            // go to first cursor and print buffer
            setCursor(firstCursorX, firstCursorY);
            printString(buffer);
            // forward to get the deleted char and backspace it
            forwardCursor();
            printChar(KEY_BKSP);
            // restore cursor
            setCursor(lastCursorX, lastCursorY);
        }
        // get the input if it's a printable character
        else if (IS_PRINTABLE(input) && len < MAX_INPUT - 1) // left for nullt
        {
            for (i = len++; i > index; i--) {
                buffer[i] = buffer[i - 1];
            }
            buffer[index++] = input;
            forwardCursor(); // forward cursor and save the pos
            lastCursorX = cursorX;
            lastCursorY = cursorY;
            // go to first cursor and print buffer
            setCursor(firstCursorX, firstCursorY);
            printString(buffer);
            // restore the pos
            setCursor(lastCursorX, lastCursorY);
        }
        // otherwise we ignore input
    }
}

