#include "../header/filesystem.h"
#include "../header/constant.h"

byte current_dir = FS_NODE_P_IDX_ROOT;
struct node_filesystem node_fs_buffer;
struct map_filesystem map_fs_buffer;
struct sector_filesystem sector_fs_buffer;

char __arg1[MAX_INPUT];
char __arg2[MAX_INPUT];
char __arg3[MAX_INPUT];
char __arg4[MAX_INPUT];
char* args[MAX_ARGS] = {__arg1, __arg2, __arg3, __arg4};
int arg_count = 0;

byte arg_cdir;
byte arg_ldir;
char name_temp[14];

char __hist0[MAX_INPUT];
char __hist1[MAX_INPUT];
char __hist2[MAX_INPUT];
char __hist3[MAX_INPUT];
char __hist4[MAX_INPUT];
char* history[MAX_HIST] = {__hist0, __hist1, __hist2, __hist3, __hist4};
int hist_length = 0;

char input_buf[MAX_INPUT];

int firstCursorX, firstCursorY, lastCursorX, lastCursorY;
