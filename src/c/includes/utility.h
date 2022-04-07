#ifndef UTILITY_H
#define UTILITY_H

#include "../library/syscall.h"

// ls.c
void ls(char* path);

// cd.c
void cd(char* path);

// mkdir.c
enum fs_retcode createFolder(byte parent, char* name);
enum fs_retcode cpFile(byte from, byte to, char* last_name, char* current_name);
void mkdir(char* path);

// cat.c
void cat(char* path);

// mv.c
void mv(char* path1, char* path2);

// cp.c
void ret_output(enum fs_retcode ret);
void cp(char* path1, char* path2);

// args.c
bool isDirectory(byte cdir);
bool checkIsFile(char* path, byte cdir);
bool checkIsDirectory(char* path, byte cdir);
bool checkIsExist(char* path, byte cdir);

void __applyPath(char* path, int dot_cnt, int len);
void parsePathArg(char* path);
void parseArgs(char* input);

#endif