#ifndef GLOBAL__H
#define GLOBAL__H

#include "constant.h"
#include "filesystem.h"

extern struct node_filesystem node_fs_buffer;
extern struct map_filesystem map_fs_buffer;
extern struct sector_filesystem sector_fs_buffer;

extern char* input_buf;
extern int firstCursorX, firstCursorY, lastCursorX, lastCursorY;
extern char* history[MAX_HIST];
extern int hist_length;

#endif