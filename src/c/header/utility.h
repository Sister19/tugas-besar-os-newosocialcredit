#ifndef UTILITY_H
#define UTILITY_H

#include "syscall.h"

extern char* args[MAX_ARGS];
extern int arg_count;

// global shell cwd
extern byte current_dir;
extern struct node_filesystem node_fs_buffer;

// args.c
extern byte arg_cdir;
extern byte arg_ldir;
extern char* name_temp;

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

bool isDirectory(byte cdir);
bool checkIsFile(char* path, byte cdir);
bool checkIsDirectory(char* path, byte cdir);
bool checkIsExist(char* path, byte cdir);

void __applyPath(char* path, int dot_cnt, int len);
void parsePathArg(char* path);
void parseArgs(char* input);

#endif