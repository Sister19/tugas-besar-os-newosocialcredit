#include "program.h"
#include "textio.h"
#include "../includes/struct_fs.h"
#include "args.h"

void exec(int segment) {
    intr(0x21, 0x5, segment, 0, 0);
}

bool initProgram(struct parsed_arg *args, int segment) {
    return intr(0x21, REG(0x4, 0x5), args, segment, 0);
}

void exit(struct shell_data* data);
void runner(struct shell_data* data) {
    int segment;
    byte buffer[512];
    struct parsed_arg args;
    parseArgs(data);
    clear(buffer, 512);
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

void exit(struct shell_data* data) {
    if (data->cwd.parse_ret == INPUT_TRAIL) { // execute multiple program
        runner(data);
    } else {
        exec(0x2000); // back to shell
    }
}
