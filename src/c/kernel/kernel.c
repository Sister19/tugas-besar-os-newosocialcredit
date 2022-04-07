#include "../system/screen.h"
#include "../system/keyboard.h"
#include "../includes/constant.h"
#include "../system/filesystem.h"
#include "kernel.h"

int main()
{
    int i = 0, j = 0;
    byte buffer[512];
    setVideoMode(VIDEO_MODE); // set video mode to 03
    printTitle();
    // put shell to memory
    for (i = 0; i < 16; i++) {
        readSector(buffer, 0x110 + i);
        for (j = 0; j < 512; j++) {
            putInMemory(0x2000, i * 512 + j, buffer[j]);
        }
    }
    fillMap(); // call fillMap function
    makeInterrupt21();
    // spawn shell
    //exec(0x2000);
    while(true);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    switch (ax) {
        case 0x0: // print stdout
            if (dx == 2)
                printStringColored(bx, cx);
            else if (dx == 3)
                printString(bx);
            else if (dx == 4)
                printChar(bx);
            else if (dx == 5)
                printCharColored(bx, cx);
            break;
        case 0x1: // keyboard input / stdin
            if (dx == 0)
                readString(bx);
            else if (dx == 1) // bx & cx should be pointer
                readKey(bx, cx);
            break;
        case 0x2: // sector mounting
            if (dx == 0)
                readSector(bx, cx);
            else if (dx == 1)
                writeSector(bx, cx);
            break;
        case 0x3: // file manip / memory
            if (dx == 0)
                read(bx, cx);
            else if (dx == 1)
                write(bx, cx);
            //else if (dx == 2)
                //ax = getNodeIdxFromParent(bx, cx);
            break;
        case 0x4: // screen manipulation
            if (dx == 0)
                clearScreen();
            break;
        case 0x5: // mouse manipulation
            if (dx == 0)
                ax = getCursorPos();
            else if (dx == 1)
                setCursor(bx, cx);
            break;
        case 0x6:
            exec(bx);
            break;
        default:
            printString("Invalid Interrupt");
            break;
    }
}
