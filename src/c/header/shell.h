#ifndef SHELL_H
#define SHELL_H

#include "filesystem.h"
#include "constant.h"
#include "std_lib.h"
#include "keyboard.h"
#include "screen.h"
#include "kernel.h"

extern char* args[MAX_ARGS];
extern int arg_count;

// global shell cwd
extern byte current_dir;
extern struct node_filesystem node_fs_buffer;

// shell.c
void printCWD();
void shell();

// args.c
extern byte arg_cdir;
extern byte arg_ldir;
extern char* name_temp;
void parseArgs(char* input);
void parsePathArg(char* path);
bool checkIsDirectory(char* path, byte cdir);
bool checkIsExist(char* path, byte cdir);
bool isDirectory(byte cdir);

// ls.c
void ls(char* path);

// cd.c
void cd(char* path);

// mkdir.c
void mkdir(char* path);

// cat.c
void cat(char* path);

// mv.c
void mv(char* path1, char* path2);

// cp.c
void ret_output(enum fs_retcode ret);
void cp(char* path1, char* path2);

#endif