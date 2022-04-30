#include "screen.h"
#include "keyboard.h"

void getCursor(int* cursorX, int* cursorY)
{
    int DX = getCursorPos();
    *cursorX = REG_L(DX);
    *cursorY = REG_H(DX);
}

void startCursor(int* x, int* y) {
    static int firstCursorX = 0, firstCursorY = 0;
    if (*x == 0 && *y == -1)
        firstCursorY--;
    else if (*x >= 0 && *y >= 0) {
        firstCursorX = *x;
        firstCursorY = *y;
    }
    *x = firstCursorX;
    *y = firstCursorY;
}

void endCursor(int* x, int* y) {
    static int lastCursorX = 0, lastCursorY = 0;
    if (*x == 0 && *y == -1)
        lastCursorY--;
    else if (*x >= 0 && *y >= 0) {
        lastCursorX = *x;
        lastCursorY = *y;
    }
    *x = lastCursorX;
    *y = lastCursorY;
}

void setCursor(int x, int y)
{
    int cx = 0, cy = -1;
    if (y > MAX_CURSOR_Y) {
        intr(INT_VIDEO, AX_VIDEO_SCROLLUP(0x01), 0x0F00, START_CURSOR, END_CURSOR);
        y = MAX_CURSOR_Y;
        endCursor(&cx, &cy);
        cx = 0, cy = -1;
        startCursor(&cx, &cy);
    }
    intr(INT_VIDEO, AX_VIDEO_SETCURSOR, 0, 0, REG(y, x));
}

void forwardCursor() {
    int cursorX, cursorY;
    getCursor(&cursorX, &cursorY);
    if (cursorX == MAX_CURSOR_X)
    {
        cursorX = 0;
        ++cursorY;
    } else {
        ++cursorX;
    }
    setCursor(cursorX, cursorY);
}

void backwardCursor() {
    int cursorX, cursorY;
    getCursor(&cursorX, &cursorY);
    if (cursorX == 0) // already leftmost
        setCursor(MAX_CURSOR_X, cursorY - 1);
    else
        setCursor(cursorX - 1, cursorY);
}

void printCharColored(char c, char color) {
    int cursorX, cursorY;
    getCursor(&cursorX, &cursorY);
    if (c == KEY_LF) {
        setCursor(0, cursorY + 1);
    } else if (c == KEY_BKSP) {
        deleteChar();
    } else if (IS_PRINTABLE(c)) {
        intr(INT_VIDEO, AX_WRITE_CHAR(c), color, 1, 0);
        forwardCursor();
    }
}

void printChar(char c) {
    printCharColored(c, COLOR_GRAY);
}

void clearScreen()
{
    /*interrupt for scrolling down (clearing screen you can say)
    16-bits register can be divided into two parts:
      ...X = |  ...H |  ...L |
    
    mode: 0x10 (write)
    AX, function code register (0x0600) (AX_VIDEO_SCROLLUP)
      AH = 0x06 (scroll up)
      AL = 0x00 (scroll up 0 lines, dependent to DX)
    BX, color register (0x0F00) (BX_VIDEO_COLOR_DEFAULT)
      BH: 0x0F (white color / foreground)
      BL: 0x00 (black color / background)
    CX, number of beginning row and column (START_CURSOR)
      CH: upper row number, 0
      CL: left column number, 0
    DX, number of ending row and column (END_CURSOR
      DH: lower row number, 24 (0x18)
      DL: right column number, 79 (0x4F)
      call interrupt 0x02 (AX_VIDEO_SETCURSOR) to reset cursor position
  */

    intr(INT_VIDEO, AX_VIDEO_SCROLLUP(0), BX_VIDEO_COLOR_DEFAULT, START_CURSOR, END_CURSOR);
    setCursor(0, 0);
}

void printString(char *buffer)
{
    int i = 0;
    // print char by char while buffer[i] is not a null-terminating character
    while (buffer[i] != nullt) {
        printChar(buffer[i]);
        i++;
    }
}

void printStringColored(char *c, int warna)
{
    /* Menulis string pada baris dan kolom dengan warna sesuai parameter fungsi */
    int i;
    i = 0;
    while (c[i] != nullt)
    {
        printCharColored(c[i], warna);
        i++;
    }
}

void deleteChar()
{
    backwardCursor();
    intr(INT_VIDEO, AX_WRITE_CHAR(KEY_SP), COLOR_GRAY, 1, 0);
}

void printTitle()
{
    printStringColored("====================================================", 0x0A); endl;
    printStringColored("||  _   _               _____ _____        _____  ||", 0x0A); endl;
    printStringColored("|| | \\ | |             |  _  /  ___|      /  __ \\ ||", 0x0A); endl;
    printStringColored("|| |  \\| | _____      _| | | \\ `--. ______| /  \\/ ||", 0x0A); endl;
    printStringColored("|| | . ` |/ _ \\ \\ /\\ / / | | |`--. \\______| |     ||", 0x0A); endl;
    printStringColored("|| | |\\  |  __/\\ V  V /\\ \\_/ /\\__/ /      | \\__/\\ ||", 0x0A); endl;
    printStringColored("|| \\_| \\_/\\___| \\_/\\_/  \\___/\\____/        \\____/ ||", 0x0A); endl;
    printStringColored("|| v.3.0.0                                        ||",  0x0A); endl;
    printStringColored("====================================================", 0x0A); endl;
    printStringColored("  Made by three dudes: ", 0x0A); endl;
    printStringColored("- 13520103 - Amar Fadil", 0x0A); endl;
    printStringColored("- 13520124 - Owen Christian Wijaya", 0x0A); endl;
    printStringColored("- 13520139 - Fachry Dennis Heraldi", 0x0A); endl;


    endl;endl;
}

void help()
{
    endl;
    printStringColored("cd <DEST>", COLOR_LIGHT_CYAN);
    printString("- Moves current working directory to destination path"); endl;
    printStringColored("ls", COLOR_LIGHT_CYAN);
    printString(" - Displays content of current directory"); endl;
    printStringColored("mv <FILE> <DEST_PATH>", COLOR_LIGHT_CYAN);
    printString(" - Moves a file to destination path"); endl;
    printStringColored("mkdir <DIR_NAME>", COLOR_LIGHT_CYAN);
    printString(" - Creates a new directory in current working directory"); endl;
    printStringColored("cat <FILE>", COLOR_LIGHT_CYAN);
    printString(" - Prints the content of a file"); endl;
    printStringColored("cp <FILE> <DEST_PATH> / <NEW_NAME>", COLOR_LIGHT_CYAN);
    printString(" - Copies a file to a path or to the same path with different name"); endl;
    endl;
}