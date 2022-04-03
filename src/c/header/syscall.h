#ifndef SYSCALL_H
#define SYSCALL_H

#include "constant.h"
#include "struct_fs.h"
#include "std_type.h"
#include "std_lib.h"

// Write endline
#define endl printChar(nullt)
// Write new space
#define sp printChar(' ')

// 0x0
void printLines(char* buf);
void printStringColored(char *buf, int col);
void printString(char *buf);
void printChar(char c);
// 0x1
void readString(char *buf);
void readKey(char* scancode, char *key);
// 0x2
void readSector(byte* buf, int sec_num);
void writeSector(byte* buf, int sec_num);
// 0x3
void read(struct file_metadata *metadata, enum fs_retcode *return_code);
void write(struct file_metadata *metadata, enum fs_retcode *return_code);
int getNodeIdxFromParent(char* name, unsigned char parent);
//0x4
void clearScreen();

#endif