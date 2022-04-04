#include "../header/syscall.h"

//#define AX_WRITE_CHAR(x) (0x0900 | x)

int main() {
    char buffer[MAX_INPUT];
    while (true) {
        printStringColored(">> ", COLOR_GREEN);
        readString(buffer); endl;
        printString(buffer); endl;
    }
    // interrupt(0x10, 0x3, 0, 0, 0);
    // interrupt(0x10, AX_WRITE_CHAR('a'), 0x7, 1, 0);
    // interrupt(0x10, AX_WRITE_CHAR('m'), 0x7, 1, 0);
}