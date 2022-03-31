#ifndef SHELL_H
#define SHELL_H

#include "filesystem.h"
#include "constant.h"
#include "std_lib.h"
#include "keyboard.h"
#include "screen.h"
#include "kernel.h"

#define MAX_ARGS 5

char __arg1[256];
char __arg2[256];
char __arg3[256];
char __arg4[256];
char __arg5[256];
char* args[MAX_ARGS] = {__arg1, __arg2, __arg3, __arg4, __arg5};
int arg_count = 0;

void parseArgs(char* args);
void shell();
void ls();
void cd(char* path);
void mkdir(char* fname);


#endif