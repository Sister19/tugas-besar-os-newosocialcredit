#ifndef __KERNEL_H
#define __KERNEL_H

#include "../includes/interrupt.h"

// Fungsi bawaan
extern void putInMemory(int segment, int address, byte b);
extern void makeInterrupt21();
extern void executeProgram(int segment);
extern int getTime();

void handleInterrupt21(int ax, int bx, int cx, int dx);

void readString(char *string);
void printTitle();
int main();

#endif