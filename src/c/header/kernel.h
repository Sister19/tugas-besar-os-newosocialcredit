#ifndef __KERNEL_H
#define __KERNEL_H

#include "std_lib.h"
#include "std_type.h"
#include "interrupt.h"

// Fungsi bawaan
extern void putInMemory(int segment, int address, byte b);
extern void makeInterrupt21();
extern void exec(int segment);

void handleInterrupt21(int ax, int bx, int cx, int dx);

void readString(char *string);
void printTitle();
int main();

#endif