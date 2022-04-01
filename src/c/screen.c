#include "header/screen.h"
#include "header/constant.h"
#include "header/keyboard.h"

void getCursor()
{
    intr(INT_VIDEO, AX_VIDEO_GETCURSOR, 0, 0, 0);
    cursorX = REG_L(DX);
    cursorY = REG_H(DX);
}

void setCursor(int x, int y)
{
    intr(INT_VIDEO, AX_VIDEO_SETCURSOR, 0, 0, REG(y, x));
}

void forwardCursor() {
    if (cursorX == MAX_CURSOR_X)
    {
        cursorX = 0;
        if (++cursorY > MAX_CURSOR_Y) {
            intr(INT_VIDEO, 0x0601, BX_VIDEO_COLOR_DEFAULT, 0x0000, END_CURSOR);
        }
    } else {
        cursorX++;
    }
    setCursor(cursorX, cursorY);
}

void setVideoMode(int mode)
{
    intr(INT_VIDEO, mode, 0, 0, 0);
}

void printCharColored(char c, char color) {
    getCursor();
    if (c == KEY_LF || c == KEY_CR) {
        setCursor(0, ++cursorY);
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

    intr(INT_VIDEO, AX_VIDEO_SCROLLUP, BX_VIDEO_COLOR_DEFAULT, START_CURSOR, END_CURSOR);
    intr(INT_VIDEO, AX_VIDEO_SETCURSOR, PAGE_NUMBER, START_CURSOR, START_CURSOR);
}

void printString(char *buffer)
{
    int i = 0;
    // print char by char while buffer[i] is not a null-terminating character
    for (i = 0; buffer[i] != nullt; i++)
    { // output selama ga \0
        printChar(buffer[i]);
    }
}

void printStringColored(char *c, int warna)
{
    /* Menulis string pada baris dan kolom dengan warna sesuai parameter fungsi */
    unsigned int i;
    i = 0;
    while (c[i] != nullt)
    {
        printCharColored(c[i], warna);
        i++;
    }
}

void deleteChar()
{
    if (cursorX == 0)
    { // already leftmost
        // delete character sequence
        // go rightmost up
        setCursor(MAX_CURSOR_X, cursorY - 1);
        // delete character. cursor will be in leftmost down again
        intr(INT_VIDEO, AX_WRITE_CHAR(KEY_SP), COLOR_GRAY, 1, 0);
    }
    else
    {
        // delete character sequence
        // backspace, reset character (thus moving the cursor again), then backspace again
        setCursor(cursorX - 1, cursorY);
        intr(INT_VIDEO, AX_WRITE_CHAR(KEY_SP), COLOR_GRAY, 1, 0);
    }
}

void printTitle()
{
    printStringColored("====================================================", COLOR_LIGHT_GREEN); endl;
    printStringColored("||  _   _               _____ _____        _____  ||", COLOR_LIGHT_GREEN); endl;
    printStringColored("|| | \\ | |             |  _  /  ___|      /  __ \\ ||", COLOR_LIGHT_GREEN); endl;
    printStringColored("|| |  \\| | _____      _| | | \\ `--. ______| /  \\/ ||", COLOR_LIGHT_GREEN); endl;
    printStringColored("|| | . ` |/ _ \\ \\ /\\ / / | | |`--. \\______| |     ||", COLOR_LIGHT_GREEN); endl;
    printStringColored("|| | |\\  |  __/\\ V  V /\\ \\_/ /\\__/ /      | \\__/\\ ||", COLOR_LIGHT_GREEN); endl;
    printStringColored("|| \\_| \\_/\\___| \\_/\\_/  \\___/\\____/        \\____/ ||", COLOR_LIGHT_GREEN); endl;
    printStringColored("|| v.1.0.0                                        ||", COLOR_LIGHT_GREEN); endl;
    printStringColored("====================================================", COLOR_LIGHT_GREEN); endl;

    printStringColored("A IF2230 Milestone 1 Project made by:", COLOR_YELLOW); endl;
    printStringColored("- 13520103 - Amar Fadil", COLOR_LIGHT_GREEN); endl;
    printStringColored("- 13520124 - Owen Christian Wijaya", COLOR_LIGHT_GREEN); endl;
    printStringColored("- 13520139 - Fachry Dennis Heraldi", COLOR_LIGHT_GREEN); endl;
    printStringColored("Type 'help' to show the help menu!", COLOR_LIGHT_CYAN); endl;

    endl;endl;
}

void help(){
    endl;endl;
    printString("==========================================================="); endl;
    printString("|                      HELP MENU                          |"); endl;
    printString("|                                                         |"); endl;
    printString("| 1. rw: Protocol to read a string and write it           |"); endl;
    printString("| on the screen.                                          |"); endl;
    printString("| 2. cls: Clear the screen and reset buffer color         |"); endl;
    printString("| to white.                                               |"); endl;
    printString("| 3. help: Show this help menu.                           |"); endl;
    printString("|                                                         |"); endl;
    printString("==========================================================="); endl;
    endl;
}