#include "../header/filesystem.h"
#include "../header/constant.h"
byte current_dir = FS_NODE_P_IDX_ROOT;
struct node_filesystem node_fs_buffer;

char __arg1[MAX_INPUT];
char __arg2[MAX_INPUT];
char __arg3[MAX_INPUT];
char __arg4[MAX_INPUT];
char* args[MAX_ARGS] = {__arg1, __arg2, __arg3, __arg4};
int arg_count = 0;

byte arg_cdir;
byte arg_ldir;
char name_temp[14];
