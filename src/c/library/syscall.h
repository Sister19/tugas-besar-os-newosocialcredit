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
void setCursPos(int x, int y);

#endif