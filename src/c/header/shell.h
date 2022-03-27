#ifndef SHELL_H
#define SHELL_H

#include "filesystem.h"
#include "constant.h"
#include "std_lib.h"

void shell();
void ls();
void mkdir(char* folder_name, char curr_dir);
void get_dir_table(char* directory_table);
void getdir(char* string, char* directory_table, char curr_dir);


#endif