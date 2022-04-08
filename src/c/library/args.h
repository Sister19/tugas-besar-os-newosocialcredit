#ifndef _ARGS_H
#define _ARGS_H

#include "../includes/struct_fs.h"
#include "../includes/constant.h"
#include "syscall.h"
#include "shell_common.h"

bool isDirectory(struct node_entry *node, byte cdir);
bool checkIsFile(struct node_entry *node, char* path, byte cdir);
bool checkIsDirectory(struct node_entry *node, char* path, byte cdir);
bool checkIsExist(char* path, byte cdir);
void parseArgs(struct shell_data *data);

// input: path, current_dir
// output: arg_cdir, arg_ldir, name_res, node
void parsePathArg(char* path, byte current_dir, byte* arg_cdir, byte* arg_ldir, char* name_res, struct node_entry *node);

#endif