#include "../header/syscall.h"

int main() {
    char buffer[512];
    printString("lol");
    readString(buffer);
    return 0;
}