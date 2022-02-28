#include "header/kernel.h"
#include "header/std_lib.h"

#define length 128
#define endl printString("\r\n");

// Stripping the header means we have to late-define the functions.
// void printString(char *buffer);
// void readString(char *buffer);
// void clearScreen();
// void printTitle();

char buffer[length + 1];

int main()
{
    interrupt(INT_VIDEO, AX_VIDEO_MODE, 0, 0, 0); // set video mode to 03
    printTitle();
    while (true)
    {
        makeInterrupt21();
        endl;
        printString("Input string (cls: clear): ");
        readString(buffer);
        if (strcmp(buffer, "cls"))
        {
            clearScreen();
        }
        else
        {
            printString("\r\nYou inputted: ");
            printString(buffer);
            printString("\r\n");
        }
    }
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    switch (AX)
    {
    case 0x0:
        printString(BX);
        break;
    case 0x1:
        readString(BX);
        break;
    default:
        printString("Invalid interrupt");
    }
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

void printStringColored(char *c, int baris, int kolom, int warna)
{
    /* Menulis string pada baris dan kolom dengan warna sesuai parameter fungsi */
    int i, b, alamat;
    i = 0;

    alamat = (baris - 1) * 80 * 2 + 2 * kolom;
    while (c[i] != nullt)
    {
        b = alamat;
        putInMemory(VID_SEGMENT, b + 0x8000, c[i]);
        putInMemory(VID_SEGMENT, b + 0x8001, warna);
        i = i + 1;
        alamat = alamat + 2;
    }
}

void readString(char *buffer)
{
    int index = 0;
    while (true)
    {
        // get interrupt for 0x16 (used to scan keyboard input)
        int input = interrupt(INT_KEYBOARD, AX_KEYBOARD_READ, 0, 0, 0);
        // If input is either CR or LF (enter), finish reading string
        if (input == 0xD || input == 0xA)
        {
            buffer[index] = nullt;
            return;
        }
        // delete input if input is backspace (0x8) and there's any buffer left
        else if (input == 0x8 && index > 0)
        {
            buffer[index] = nullt;
            index--;
            // delete character sequence
            // backspace, reset character (thus moving the cursor again), then backspace again
            printChar(0x8); printChar(0x0); printChar(0x8);
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

void clearScreen()
{
    interrupt(
        INT_VIDEO,
        AX_VIDEO_SCROLLUP,
        BX_VIDEO_COLOR_DEFAULT,
        START_CURSOR,
        END_CURSOR
    );

    interrupt(
        INT_VIDEO,
        AX_VIDEO_SETCURSOR,
        PAGE_NUMBER,
        START_CURSOR,
        START_CURSOR
    );
}

void printTitle()
{
    printStringColored(" ________   _______   ___       __   ________  ________      ", 1, 0, 0x0A);
    endl;
    printStringColored("|\\   ___  \\|\\  ___ \\ |\\  \\     |\\  \\|\\   __  \\|\\   ____\\    "
                       " ",
                       2, 0, 0x0A);
    endl;
    printStringColored("\\ \\  \\ \\  \\ \\   __/|\\ \\  \\    \\ \\  \\ \\  \\|\\  \\ \\  "
                       "\\___|_    ",
                       3, 0, 0x0A);
    endl;
    printStringColored(" \\ \\  \\ \\  \\ \\  \\_|/_\\ \\  \\  __\\ \\  \\ \\  \\\\  \\ \\_____ "
                       " \\   ",
                       4, 0, 0x0A);
    endl;
    printStringColored("  \\ \\  \\ \\  \\ \\  \\_|\\ \\ \\  \\|\\__\\_\\  \\ \\  \\\\  "
                       "\\|____|\\  \\  ",
                       5, 0, 0x0A);
    endl;
    printStringColored("   \\ \\__\\ \\__\\ \\_______\\ \\____________\\ \\_______\\____\\_\\  "
                       "\\ ",
                       6, 0, 0x0A);
    endl;
    printStringColored("    \\|__| \\|__|\\|_______|\\|____________|\\|_______|\\_________\\", 7, 0, 0x0A);
    endl;
    printStringColored("                                                 \\|_________|", 8, 0, 0x0A);
    endl;
    endl;
    printStringColored("Quick guide of currently available functions: ", 12, 0, 0x09);
    endl;
    printStringColored("1. Enter a string and the system will print it", 13, 0, 0x0B);
    endl;
    printStringColored("2. Enter 'cls' to clear the terminal ",
                       14, 0, 0x0B);
    endl;
    endl;
}

void printChar(char c) {
    interrupt(INT_VIDEO, AX_TELETYPE_WRITE(c), 0, 0, 0);
}