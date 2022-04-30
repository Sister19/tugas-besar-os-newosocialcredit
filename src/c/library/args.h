#ifndef __ARGS_LIB_H
#define __ARGS_LIB_H

#include "../includes/struct_lib.h"
#include "../includes/struct_fs.h"
#include "../includes/interrupt.h"

bool isDirectory(struct node_entry *node, byte cdir);

bool checkIsFile(struct node_entry *node, char* path, byte cdir);

bool checkIsDirectory(struct node_entry *node, char* path, byte cdir);

bool checkIsExist(char* path, byte cdir);

void parsePathArg(char* path, byte current_dir, struct parsed_arg *res);

void parseArgs(struct shell_data *data);

#endif