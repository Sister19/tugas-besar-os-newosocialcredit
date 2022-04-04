#include "../header/shell.h"
#include "../header/interrupt.h"
struct node_filesystem node_fs_buffer;

byte current_dir = FS_NODE_P_IDX_ROOT;
char __arg1[MAX_INPUT];
char __arg2[MAX_INPUT];
char __arg3[MAX_INPUT];
char __arg4[MAX_INPUT];
char* args[MAX_ARGS] = {__arg1, __arg2, __arg3, __arg4};
int arg_count = 0;

byte arg_cdir;
byte arg_ldir;
char name_temp[14];

void _printCWDRec(byte dir) {
    if (dir == FS_NODE_P_IDX_ROOT) {
        printCharColored('/', COLOR_LIGHT_CYAN);
    } else {
        _printCWDRec(node_fs_buffer.nodes[dir].parent_node_index);
        printStringColored(node_fs_buffer.nodes[dir].name, COLOR_LIGHT_CYAN);
        printCharColored('/', COLOR_LIGHT_CYAN);
    }
}

void printCWD() {
    readNodeFs(&node_fs_buffer);
    _printCWDRec(current_dir);
}

bool __checkArgCount(int min_arg) {
    if (arg_count < min_arg) {
        printStringColored("Not enough arguments.\n", COLOR_LIGHT_RED);
        return false;
    }
    return true;
}

int main() {
    // while (true){
    char input_buf[MAX_INPUT];
    int i;
    printString(">> ");
    while(1);
    // while (true) {
    //     printStringColored("NewOS:", COLOR_LIGHT_GREEN);
    //     printCWD();
    //     printString(" >> ");
    //     readString(input_buf);
    //     parseArgs(input_buf);
    //     endl;
    //     if (arg_count > MAX_ARGS) {
    //         printString("Too many arguments\n");
    //         continue;
    //     } else if (arg_count == 0) {
    //         continue;
    //     }
    //     if (strcmp(args[0], "ls")) {
    //         if (arg_count > 1)
    //             ls(args[1]);
    //         else
    //             ls(0);
    //     } else if (strcmp(args[0], "mkdir")) {
    //         if (__checkArgCount(2))
    //             mkdir(args[1]);
    //     } else if (strcmp(args[0], "cd")) {
    //         if (__checkArgCount(2))
    //             cd(args[1]);
    //     } else if (strcmp(args[0], "cls")) {
    //         clearScreen();
    //     } else if (strcmp(args[0], "cat")) {
    //         if (__checkArgCount(2))
    //             cat(args[1]);
    //     } else if (strcmp(args[0], "mv")) {
    //         if (__checkArgCount(3))
    //             mv(args[1], args[2]);
    //     } else if (strcmp(args[0], "cp")){
    //         if (__checkArgCount(3))
    //             cp(args[1], args[2]);
    //     }
    // }
}
