#include "header/screen.h"
#include "header/constant.h"

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

void setVideoMode(int mode)
{
    intr(INT_VIDEO, mode, 0, 0, 0);
}

void printChar(char c)
{
    intr(INT_VIDEO, AX_TELETYPE_WRITE(c), 0, 0, 0);
}

void clearScreen()
{
    intr(INT_VIDEO, AX_VIDEO_SCROLLUP, BX_VIDEO_COLOR_DEFAULT, START_CURSOR, END_CURSOR);
    intr(INT_VIDEO, AX_VIDEO_SETCURSOR, PAGE_NUMBER, START_CURSOR, START_CURSOR);
}

void printString(char *buffer)
{
    int i = 0;
    // print char by char
    for (i = 0; buffer[i] != nullt; i++)
    { // output selama ga \0
        printChar(buffer[i]);
    }
}

void deleteChar()
{
    getCursor();
    if (cursorX == 0)
    { // already leftmost
        // delete character sequence
        // go rightmost up
        setCursor(MAX_CURSOR_X, cursorY - 1);
        // delete character. cursor will be in leftmost down again
        intr(INT_VIDEO, AX_WRITE_CHAR(nullt), PAGE_NUMBER, 1, 0);
    }
    else
    {
        // delete character sequence
        // backspace, reset character (thus moving the cursor again), then backspace again
        printChar(0x8);
        intr(INT_VIDEO, AX_WRITE_CHAR(nullt), PAGE_NUMBER, 1, 0);
    }
}
