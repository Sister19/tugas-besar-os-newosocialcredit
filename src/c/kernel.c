#include "header/kernel.h"
#include "header/screen.h"
#include "header/constant.h"
#include "header/std_lib.h"
#include "header/keyboard.h"


int main()
{
    setVideoMode(VIDEO_MODE);
    printTitle();
    while (true)
    {
        char buffer[length + 1];
        char command[length + 1];
        makeInterrupt21(); endl;
        getCursor();
        printString("Input command >>> ");
        readString(command);
        endl;
        // clear screen if cls
        if (strcmp(command, "cls"))
        {
            clearScreen();
            printString("Screen cleared!"); endl;
        }
        else if (strcmp(command, "help"))
        {
            help();
        }
        else if (strcmp(command, "rw")){
            printString("Input a string: ");
            readString(buffer);
            printString("\r\nYou inputted: ");
            printString(buffer);
            printString("\r\n");
        }
        else
        {
            printString("Unknown command, use 'help' to show available commands.");
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
