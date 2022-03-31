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

void forwardCursor() {
    if (cursorX == MAX_CURSOR_X)
    {
        cursorX = 0;
        cursorY++;
    } else {
        cursorX++;
    }
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
    int i, b, alamat;
    i = 0;
    getCursor();
    while (c[i] != nullt)
    {
        alamat = (cursorY * 80 + cursorX) * 2;
        putInMemory(VID_SEGMENT, alamat + 0x8000, c[i]);
        putInMemory(VID_SEGMENT, alamat + 0x8001, warna);
        forwardCursor();
        i++;
    }
    setCursor(cursorX, cursorY);
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

void printTitle()
{
    printStringColored("====================================================", 0x0A); endl;
    printStringColored("||  _   _               _____ _____        _____  ||", 0x0A); endl;
    printStringColored("|| | \\ | |             |  _  /  ___|      /  __ \\ ||", 0x0A); endl;
    printStringColored("|| |  \\| | _____      _| | | \\ `--. ______| /  \\/ ||", 0x0A); endl;
    printStringColored("|| | . ` |/ _ \\ \\ /\\ / / | | |`--. \\______| |     ||", 0x0A); endl;
    printStringColored("|| | |\\  |  __/\\ V  V /\\ \\_/ /\\__/ /      | \\__/\\ ||", 0x0A); endl;
    printStringColored("|| \\_| \\_/\\___| \\_/\\_/  \\___/\\____/        \\____/ ||", 0x0A); endl;
    printStringColored("|| v.1.0.0                                        ||", 0x0A); endl;
    printStringColored("====================================================", 0x0A); endl;

    printStringColored("A IF2230 Milestone 1 Project made by:", 0x0E); endl;
    printStringColored("- 13520103 - Amar Fadil", 0x0A); endl;
    printStringColored("- 13520124 - Owen Christian Wijaya", 0x0A); endl;
    printStringColored("- 13520139 - Fachry Dennis Heraldi", 0x0A); endl;
    printStringColored("Type 'help' to show the help menu!", 0x0B); endl;

    endl;endl;
}

void help(){
    endl;endl;
    getCursor();
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