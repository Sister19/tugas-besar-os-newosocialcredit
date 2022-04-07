#ifndef SHELL_COMMON_H
#define SHELL_COMMON_H

#include "../includes/constant.h"
#include "../includes/std_type.h"

void spawnShell();

// 256 * 8 (2048 / 4 sector) args
struct shell_arguments {
    char argv[MAX_ARGS][MAX_INPUT];
};

// 256 * 7 + 1 history_index (2048/4 sector) history
struct shell_history {
    char history[MAX_HIST][MAX_INPUT];
};

// 256 + 1 + 2 + 2 (512/1 sector) input
struct shell_cwd {
    char input[MAX_INPUT];
    byte current_dir;
    int history_length;
    int arg_count;
};

// 256 + 1 + 2 + 2 (1 sector) + 4 sector arg + 4 sector hist
// shell data starts from sector 0x120 
struct shell_data {
    struct shell_cwd cwd;
    struct shell_arguments arg;
    struct shell_history hist;
};

void getShellData(struct shell_data *data);
void setShellData(struct shell_data *data);

#endif
