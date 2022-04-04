#include "header/filesystem.h"
#include "header/constant.h"

struct node_filesystem node_fs_buffer;
struct map_filesystem map_fs_buffer;
struct sector_filesystem sector_fs_buffer;

char __hist0[MAX_INPUT];
char __hist1[MAX_INPUT];
char __hist2[MAX_INPUT];
char __hist3[MAX_INPUT];
char __hist4[MAX_INPUT];
char* history[MAX_HIST] = {__hist0, __hist1, __hist2, __hist3, __hist4};
int hist_length = 0;

int firstCursorX, firstCursorY, lastCursorX, lastCursorY;
