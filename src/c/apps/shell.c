#include "../library/program.h"
#include "../library/textio.h"
#include "../library/fileio.h"
#include "../library/string.h"
#include "../library/syscall.h"

void printCWD(byte current_dir);

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
        if (strcmp("cls", data.cwd.input)){
            cls();
        } else if (strcmp("help", data.cwd.input)) {
            endl;
            putsc("cd <DEST>", COLOR_LIGHT_CYAN);
            puts("- Moves current working directory to destination path"); endl;
            putsc("ls", COLOR_LIGHT_CYAN);
            puts(" - Displays content of current directory"); endl;
            putsc("mv <FILE> <DEST_PATH>", COLOR_LIGHT_CYAN);
            puts(" - Moves a file to destination path"); endl;
            putsc("mkdir <DIR_NAME>", COLOR_LIGHT_CYAN);
            puts(" - Creates a new directory in current working directory"); endl;
            putsc("cat <FILE>", COLOR_LIGHT_CYAN);
            puts(" - Prints the content of a file"); endl;
            putsc("cp <FILE> <DEST_PATH> / <NEW_NAME>", COLOR_LIGHT_CYAN);
            puts(" - Copies a file to a path or to the same path with different name"); endl;
            endl;
        } else {
            runner(&data);
        }
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
