#ifndef _ARGS_H
#define _ARGS_H

#include "../includes/struct_fs.h"
#include "../includes/struct_lib.h"

bool isDirectory(struct node_entry *node, byte cdir);
void parseArgs(struct shell_data *data);

// input: path, current_dir
// output: arg_cdir, arg_ldir, name_res, node
void parsePathArg(char* path, byte current_dir, struct parsed_arg *res);

#endif