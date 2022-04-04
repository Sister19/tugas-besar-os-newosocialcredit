#include "header/screen.h"
#include "header/keyboard.h"
#include "header/constant.h"
#include "header/filesystem.h"
#include "header/kernel.h"

int main()
{
    intr(INT_VIDEO, VIDEO_MODE, 0, 0, 0); // set video mode to 03
    fillMap(); // call fillMap function
    printTitle();
    shell();
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    switch (ax) {
        case 0x0:
            printString(bx);
            break;
        case 0x1:
            readString(bx);
            break;
        case 0x2:
            readSector(bx, cx);
            break;
        case 0x3:
            writeSector(bx, cx);
            break;
        case 0x4:
            read(bx, cx);
            break;
        case 0x5:
            write(bx, cx);
            break;
        default:
            printString("Invalid Interrupt");
    }
}
