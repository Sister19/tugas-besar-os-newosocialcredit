#ifndef SHELL_H
#define SHELL_H

#include "filesystem.h"
#include "constant.h"
#include "std_lib.h"
#include "keyboard.h"
#include "screen.h"
#include "kernel.h"

#define MAX_ARGS 4

char __arg1[MAX_INPUT];
char __arg2[MAX_INPUT];
char __arg3[MAX_INPUT];
char __arg4[MAX_INPUT];
char* args[MAX_ARGS] = {__arg1, __arg2, __arg3, __arg4};
int arg_count = 0;

void printCWD();
void parseArgs(char* args);
void shell();
void ls();
void cd(char* path);
void mkdir(char* fname);
void cat(char* fname);


#endif