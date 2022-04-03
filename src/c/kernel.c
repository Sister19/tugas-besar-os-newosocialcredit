#include "header/screen.h"
#include "header/keyboard.h"
#include "header/constant.h"
#include "header/filesystem.h"
#include "header/kernel.h"

int main()
{
    int i = 0;
    byte buffer[8192];
    clear(buffer, 8192);
    for (i = 0; i < 16; i++) {
        readSector(buffer + 512 * i, 0x110 + i);
    }
    for (i = 0; i < 8192; i++) {
        putInMemory(0x2000, i, buffer[i]);
    }
    setVideoMode(VIDEO_MODE); // set video mode to 03
    printTitle();
    fillMap(); // call fillMap function
    makeInterrupt21();
    printString("yes!");
    exec(0x2000);
    return 0;
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    switch (ax) {
        case 0x0: // print stdout
            if (dx == 1)
                printLines(bx);
            else if (dx == 2)
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
        case 0x3: // file manip
            if (dx == 0)
                read(bx, cx);
            else if (dx == 1)
                write(bx, cx);
            else if (dx == 2)
                ax = getNodeIdxFromParent(bx, cx);
            break;
        case 0x4: // screen manipulation
            if (dx == 0)
                clearScreen();
            break;
        default:
            printString("Invalid Interrupt");
            break;
    }
}
