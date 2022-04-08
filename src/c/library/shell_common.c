#include "shell_common.h"
#include "syscall.h"
#include "args.h"
#include "program.h"

void runner(struct shell_data* data) {
    int segment;
    parseArgs(data);
    if (
        data->cwd.arg_count > 0 &&
        (data->cwd.parse_ret == INPUT_TRAIL || data->cwd.parse_ret == INPUT_END)
    ) {
        segment = 0x3000 + 0x1000 * data->cwd.prog_count;
        if(initProgram(data->arg.argv[0], segment)) {
            ++data->cwd.prog_count;
            setShellData(data);
            exec(segment);
        }
    } else if (data->cwd.parse_ret == UNCLOSED_STRING) {
        putsc("Unclosed string in input!\n", COLOR_LIGHT_RED);
    } else if (data->cwd.parse_ret == ARG_LIMIT) {
        putsc("Arguments reached limit! (8 max)\n", COLOR_LIGHT_RED);
    }
    exit(data);
}

void getShellData(struct shell_data* data) {
    int i;
    reads(&(data->cwd), 0x120);
    for (i = 0; i < 4; i++) {
        reads(data->arg.argv[i], 0x121 + i);
    }
}

void setShellData(struct shell_data* data) {
    int i;
    writes(&(data->cwd), 0x120);
    for (i = 0; i < 4; i++) {
        writes(data->arg.argv[i], 0x121 + i);
    }
}
