#include "../library/program.h"

void printCWD(byte current_dir);
bool __checkArgCount(int arg_count, int min_arg);

int main() {
    struct shell_data data;
    getShellData(&data);
    data.cwd.prog_count = 0;
    data.cwd.cur_inp_idx = 0;
    while (true) {
        putsc("NewOS:", COLOR_LIGHT_GREEN);
        printCWD(data.cwd.current_dir);
        puts(" >> ");
        gets(data.cwd.input);
        endl;
        runner(&data);
    }
}

void _printCWDRec(struct node_filesystem* node_fs_buffer, byte dir) {
    if (dir == FS_NODE_P_IDX_ROOT) {
        putc('/', COLOR_LIGHT_CYAN);
    } else {
        _printCWDRec(node_fs_buffer, node_fs_buffer->nodes[dir].parent_node_index);
        putsc(node_fs_buffer->nodes[dir].name, COLOR_LIGHT_CYAN);
        putc('/', COLOR_LIGHT_CYAN);
    }
}

void printCWD(byte current_dir) {
    struct node_filesystem node_fs_buffer;
    readsNode(&node_fs_buffer);
    _printCWDRec(&node_fs_buffer, current_dir);
}

bool __checkArgCount(int arg_count, int min_arg) {
    if (arg_count < min_arg) {
        putsc("Not enough arguments.\n", COLOR_LIGHT_RED);
        return false;
    }
    return true;
}
