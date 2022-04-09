#include "program.h"
#include "../includes/struct_fs.h"
#include "../includes/interrupt.h"

void exec(int segment) {
    intr(0x21, 0x5, segment, 0, 0);
}

bool initProgram(char* name, int segment) {
    int i;
    struct file_metadata bin;
    enum fs_retcode ret;
    byte buffer[512 * 16];
    strncpy(bin.node_name, name, 14);
    bin.buffer = buffer;
    clear(buffer, 512*16);
    bin.parent_index = 0;
    read(&bin, &ret);
    if (ret == 0) {
        for (i = 0; i < 8192; i++) {
            putInMemory(segment, i, buffer[i]);
        }
        return true;
    } else {
        return false;
    }
}

void exit(struct shell_data* data) {
    if (data->cwd.parse_ret == INPUT_TRAIL) { // execute multiple program
        runner(data);
    } else {
        exec(0x2000); // back to shell
    }
}