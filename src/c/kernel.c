#include "header/kernel.h"
#include "header/screen.h"
#include "header/constant.h"
#include "header/std_lib.h"

#define length 128

char buffer[length + 1];

int main()
{
    setVideoMode(VIDEO_MODE);
    printTitle();
    while (true)
    {
        makeInterrupt21();
        endl;
        printString("Input string (cls: clear): ");
        readString(buffer);
        // clear screen if cls
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
