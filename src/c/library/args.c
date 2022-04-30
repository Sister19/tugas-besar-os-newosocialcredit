#include "args.h"
#include "textio.h"

bool isDirectory(struct node_entry *node, byte cdir) {
    return intr(0x21, REG(0x3, 0x5), node, cdir, 0);
}


bool checkIsFile(struct node_entry *node, char* path, byte cdir) {
    if (isDirectory(node, cdir)) {
        putsc("Not a file: ", COLOR_LIGHT_RED);
        putsc(path, COLOR_LIGHT_BLUE); endl; // is not a file
        return false;
    }
    return true;
}

bool checkIsDirectory(struct node_entry *node, char* path, byte cdir) {
    if (!isDirectory(node, cdir)) {
        putsc("Not a directory: ", COLOR_LIGHT_RED);
        putsc(path, COLOR_LIGHT_BLUE); endl; // is not a directory
        return false;
    }
    return true;
}

bool checkIsExist(char* path, byte cdir) {
    if (cdir == IDX_NODE_UNDEF) {
        putsc("No such file or directory: ", COLOR_LIGHT_RED);
        putsc(path, COLOR_LIGHT_BLUE); endl; // no such file or directory
        return false;
    }
    return true;
}

void parsePathArg(char* path, byte current_dir, struct parsed_arg *res) {
    intr(0x21, REG(0x2, 0x5), path, current_dir, res);
}

void parseArgs(struct shell_data *data) {
    intr(0x21, REG(0x1, 0x5), data, 0, 0);
}
