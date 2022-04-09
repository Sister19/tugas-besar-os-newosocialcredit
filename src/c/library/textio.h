#ifndef TEXTIO__H
#define TEXTIO__H

#include "../includes/std_type.h"

// Write endline
#define endl put('\n')
// Write new space
#define sp put(' ')

// 0x0
void putsc(char *buf, int col);
void puts(char *buf);
void put(char c);
void putc(char c, char color);
void putl(char *buffer);

// 0x1
void gets(char *buf);
void get(char* scancode, char *key);
void getb(char* scancode, char *key);

#endif