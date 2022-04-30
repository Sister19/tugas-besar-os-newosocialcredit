#ifndef __STRUCT_LIB_H
#define __STRUCT_LIB_H

#include "constant.h"
#include "std_type.h"

struct parsed_arg {
    byte arg_cdir;
    byte arg_ldir;
    char name_res[14];
    struct node_entry *node;
    bool is_traversed; // is traversed with apply path (absolute/relative)
};

// 256 * 8 (2048 / 4 sector) args
struct shell_arguments {
    char argv[MAX_ARGS][MAX_INPUT];
};

enum parser_retcode {
    INPUT_TRAIL = 0,
    INPUT_END = 1,
    UNCLOSED_STRING = 2,
    ARG_LIMIT = 3
};

// 256 + 1 + 2 + 2 (512/1 sector) input
struct shell_cwd {
    char input[MAX_INPUT];
    byte current_dir;
    int cur_inp_idx;
    int arg_count;
    int prog_count;
    enum parser_retcode parse_ret;
};

// 256 + 1 + 2 + 2 (1 sector) + 4 sector arg + 4 sector hist
// shell data starts from sector 0x120 
struct shell_data {
    struct shell_cwd cwd;
    struct shell_arguments arg;
};

#endif