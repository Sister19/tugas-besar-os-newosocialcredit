#ifndef PROGRAM__H
#define PROGRAM__H

#include "../library/shell_common.h"
#include "args.h"

extern void putInMemory(int segment, int address, byte b);
extern void executeProgram(int segment);

bool __init(char* name, int segment, byte cdir);
bool initProgram(struct parsed_arg *args, int segment);

#endif