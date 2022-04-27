#include "shell_common.h"
#include "args.h"
#include "textio.h"
#include "program.h"
#include "fileio.h"

void runner(struct shell_data* data) {
    int segment;
    struct parsed_arg args;
    parseArgs(data);
    if (
        data->cwd.arg_count > 0 &&
        (data->cwd.parse_ret == INPUT_TRAIL || data->cwd.parse_ret == INPUT_END)
    ) {
        // since we only use one program at a time, we cycle use 0x3000 and 0x4000
        // thus we can supply infinite amount of multi program
        segment = 0x3000 + 0x1000 * (data->cwd.prog_count % 2);
        parsePathArg(data->arg.argv[0], data->cwd.current_dir, &args);
        if(initProgram(&args, segment)) {
            ++data->cwd.prog_count;
            setShellData(data);
            exec(segment);
        } else {
            putsc("No program found: ", COLOR_LIGHT_RED);
            puts(data->arg.argv[0]); endl;
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
