#ifndef SYSCALL_H
#define SYSCALL_H

#include "../includes/constant.h"
#include "../includes/struct_fs.h"
#include "std_lib.h"
#include "../includes/interrupt.h"

// 0x4
void cls();
// 0x5
void getCurPos(int *x, int *y);
void setCurPos(int x, int y);
void moveCursor(bool backward);
// Sleep in dur ms
void sleep(int dur);
int getTime();
#endif