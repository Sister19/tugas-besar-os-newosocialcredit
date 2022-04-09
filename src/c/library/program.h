#ifndef PROGRAM__H
#define PROGRAM__H

#include "shell_common.h"
#include "args.h"

void exec(int segment);
bool initProgram(char* name, int segment);
void exit(struct shell_data* data);

#endif