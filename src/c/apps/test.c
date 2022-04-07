#include "../header/shell_common.h"
#include "../header/syscall.h"

void printCWD(byte current_dir);

int main() {
    struct shell_data data;
    char input_buf[MAX_INPUT];
    // getShellData(&data);
    // if (data.cwd.current_dir == 0)
    //     data.cwd.current_dir = FS_NODE_P_IDX_ROOT;
    while (true) {
        printStringColored("NewOS:", COLOR_LIGHT_GREEN);
        // printCWD(data.cwd.current_dir);
        printString(" >> ");
        readString(input_buf);
        // setShellData(&data);
    }
}

void _printCWDRec(struct node_filesystem* node_fs_buffer, byte dir) {
    if (dir == FS_NODE_P_IDX_ROOT) {
        printCharColored('/', COLOR_LIGHT_CYAN);
    } else {
        _printCWDRec(node_fs_buffer, node_fs_buffer->nodes[dir].parent_node_index);
        printStringColored(node_fs_buffer->nodes[dir].name, COLOR_LIGHT_CYAN);
        printCharColored('/', COLOR_LIGHT_CYAN);
    }
}

void printCWD(byte current_dir) {
    struct node_filesystem node_fs_buffer;
    readNodeFs(&node_fs_buffer);
    _printCWDRec(&node_fs_buffer, current_dir);
}

bool __checkArgCount(int arg_count, int min_arg) {
    if (arg_count < min_arg) {
        printStringColored("Not enough arguments.\n", COLOR_LIGHT_RED);
        return false;
    }
    return true;
}