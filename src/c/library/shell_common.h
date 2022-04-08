#ifndef SHELL_COMMON_H
#define SHELL_COMMON_H

#include "../includes/constant.h"
#include "../includes/std_type.h"

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

void runner(struct shell_data* data);
void getShellData(struct shell_data *data);
void setShellData(struct shell_data *data);

#endif
