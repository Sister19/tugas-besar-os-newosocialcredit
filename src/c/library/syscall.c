#include "syscall.h"

// 0x4
void cls(){
    intr(0x21, 0x4, 0, 0, 0);
}
void getCurPos(int* x, int *y) {
    int DX = intr(0x21, REG(0x1, 0x4), 0, 0, 0);
    *x = REG_L(DX);
    *y = REG_H(DX);
}
void setCurPos(int x, int y) {
    intr(0x21, REG(0x2, 0x4), x, y, 0);
}

void moveCursor(bool backward) {
    intr(0x21, REG(0x3, 0x4), 0, 0, backward);
}

// Sleep in dur ms
void sleep(int dur) {
    intr(0x21, 0x6, dur, 0, 0);
}

int getTime() {
    return intr(0x21, REG(0x1, 0x6), 0, 0, 0);
}