#include "../system/screen.h"
#include "../system/keyboard.h"
#include "../includes/constant.h"
#include "../system/filesystem.h"
#include "../system/program.h"
#include "kernel.h"

int main()
{
    bool is_title_avail;
    makeInterrupt21();
    intr(INT_VIDEO, VIDEO_MODE, 0, 0, 0); // set video mode to 03
    fillMap(); // call fillMap function
    // find title and read it
    if (!__init("shell", 0x2000, 0x0)) {
        printStringColored("Error loading OS...\nCan't find shell executable. Please rebuild the OS and include shell app.", COLOR_LIGHT_RED);
    } else {
        is_title_avail = __init("title", 0x3000, 0x0);
        executeProgram((is_title_avail ? 0x3000 : 0x2000));
    }
    while(true);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    switch (REG_L(ax)) {
        case 0x0: // print stdout
            switch (REG_H(ax)) {
                case 0:
                    printStringColored(bx, cx);
                    break;
                case 1:
                    printString(bx);
                    break;
                case 2:
                    printChar(bx);
                    break;
                case 3:
                    printCharColored(bx, cx);
                    break;
                default:
                    break;
            }
            break;
        case 0x1: // keyboard input / stdin
            switch (REG_H(ax)) {
                case 0:
                    readString(bx, cx, dx);
                    break;
                case 1:
                    readKey(bx, cx);
                    break;
                case 2:
                    readPress(bx, cx);
                    break;
                default:
                    break;
            }
            break;
        case 0x2: // sector mounting
            switch (REG_H(ax)) {
                case 0:
                    readSector(bx, cx);
                    break;
                case 1:
                    writeSector(bx, cx);
                    break;
                default:
                    break;
            }
            break;
        case 0x3: // file manip / memory
            switch (REG_H(ax)) {
                case 0:
                    read(bx, cx);
                    break;
                case 1:
                    write(bx, cx);
                    break;
                case 2:
                    putInMemory(bx, cx, dx);
                    break;
                case 3:
                    ax = getNodeIdxFromParent(bx, cx, dx);
                    break;
                default:
                    break;
            }
            break;
        case 0x4: // screen / cursor manipulation
            switch (REG_H(ax)) {
                case 0:
                    clearScreen();
                    break;
                case 1:
                    ax = getCursorPos();
                    break;
                case 2:
                    if (dx == 0) {
                        intr(INT_VIDEO, AX_VIDEO_SETCURSOR, 0, 0, REG(cx, bx));
                    } else {
                    // use the setCursor with screen aware
                        setCursor(bx, cx);
                    }
                    break;
                case 3:
                    if (dx == 0)
                        forwardCursor();
                    else
                        backwardCursor();
                    break;
                default:
                    break;
            }
            break;
        case 0x5: // program and arguments utils
            switch (REG_H(ax)) {
                case 0x0:
                    executeProgram(bx);
                    break;
                case 0x1:
                    parseArgs(bx);
                    break;
                case 0x2:
                    parsePathArg(bx, cx, dx);
                    break;
                case 0x3:
                    ax = isDirectory(bx, cx);
                    break;
                case 0x4:
                    ax = initProgram(bx, cx);
                    break;
                case 0x9:
                    getShellData(bx);
                    break;
                case 0xA:
                    setShellData(bx);
                    break;
                default:
                    break;
            }
            break;
        case 0x6:
            switch (REG_H(ax)) {
                case 0:
                    intr(0x15, 0x8600, 0, bx, 0); // vevesleef
                    break;
                case 1:
                    ax = getTime();
                    break;
                default:
                    break;
            }
            break;
        default:
            printString("Invalid Interrupt");
            break;
    }
}
