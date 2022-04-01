#include "header/shell.h"
byte current_dir = FS_NODE_P_IDX_ROOT;
struct node_filesystem node_fs_buffer;

void mkdir(char* fname){
    struct file_metadata* metadata;
    enum fs_retcode* ret;
    int AX;
    memcpy(metadata->parent_index, 0x00, 0x01);
    // printString(fname);
    strcpy(metadata->node_name, fname);
    memcpy(&metadata->parent_index, current_dir, sizeof(current_dir));
    metadata->filesize = 0;
    
    // interrupt(0x21, AX, metadata->buffer, metadata->node_name, *ret);
    write(metadata, ret);

    printString(itoa(ret)); endl;

    switch (*ret)
    {
        case 0:
            printString("Directory created successfully!"); endl;
            break;
        default:
            printString("Error"); endl;
            break;
    }
}

void _printCWDRec(byte dir) {
    if (dir == FS_NODE_P_IDX_ROOT) {
        printCharColored('/', COLOR_LIGHT_GREEN);
    } else {
        _printCWDRec(node_fs_buffer.nodes[dir].parent_node_index);
        printStringColored(node_fs_buffer.nodes[dir].name, COLOR_LIGHT_GREEN);
        printCharColored('/', COLOR_LIGHT_GREEN);
    }
}

void printCWD() {
    readNodeFs(&node_fs_buffer);
    _printCWDRec(current_dir);
}

bool applyPath(char *dirname, char* path, byte* cdir, int dot_cnt, int len) {
    if (dot_cnt == 2 && len == 2) {
        if (*cdir != FS_NODE_P_IDX_ROOT) // if not root, we can go up
            *cdir = node_fs_buffer.nodes[*cdir].parent_node_index;
    } else if (!(len == 1 && dirname[0] == '.') && len > 0) { // at least it has length > 0
        dirname[len] = nullt;
        *cdir = getNodeIdxFromParent(&node_fs_buffer, dirname, *cdir);
        if (*cdir == IDX_NODE_UNDEF) { // not found
            printStringColored("No such file or directory: ", COLOR_LIGHT_RED);
            printStringColored(path, COLOR_LIGHT_BLUE); endl;
            return false;
        } else if (node_fs_buffer.nodes[*cdir].sector_entry_index != FS_NODE_S_IDX_FOLDER){
            printStringColored("Not a directory: ", COLOR_LIGHT_RED);
            printStringColored(path, COLOR_LIGHT_BLUE); endl; // is not a directory
            return false;
        }
    }
    return true;
}

byte parsePathArg(char* path) {
    char name_temp[14];
    byte dot_cnt = 0;
    int i = 0, len = 0;
    byte cdir = current_dir;
    if (path[i] == '/' || path[i] == '\\') {
        cdir = FS_NODE_P_IDX_ROOT;
        i++;
    }
    while (path[i] != '\0') {
        if (path[i] == '.') {
            dot_cnt++;
            name_temp[len++] = path[i];
        } else if (path[i] == '/' || path[i] == '\\') {
            if (!applyPath(name_temp, path, &cdir, dot_cnt, len))
                return IDX_NODE_UNDEF;
            len = 0;
            dot_cnt = 0;
        } else {
            name_temp[len++] = path[i];
        }
        i++;
    }
    if (len > 0) { // remains.
        if (!applyPath(name_temp, path, &cdir, dot_cnt, len))
            return IDX_NODE_UNDEF;
    }
    return cdir;
}

void ls(char* path){
    int i = 0, cnt = 0;
    byte cdir = current_dir;
    readNodeFs(&node_fs_buffer);
    if (path != 0) {
        cdir = parsePathArg(path);
        if (cdir == IDX_NODE_UNDEF)
            return;
    }
    while (i < 64) {
        struct node_entry node = node_fs_buffer.nodes[i];
        if (node.name[0] != nullt && node.parent_node_index == cdir) {
            // printString(node.name); sp; sp;
            if (node.sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                printStringColored(node.name, COLOR_LIGHT_GREEN); sp; sp;
            } else {
                printString(node.name); sp; sp;
            }
            cnt++;
        }
        i++;
    }
    if (cnt > 0){
        endl;
    }
}

void cd(char* path) {
    byte cdir;
    readNodeFs(&node_fs_buffer);
    cdir = parsePathArg(path);
    if (cdir != IDX_NODE_UNDEF)
        current_dir = cdir;
}

void parseArgs(char* input) {
    int i = 0, k = 0;
    bool strflag = false;
    arg_count = 0;
    while (input[i] != '\0') {
        if (arg_count > MAX_ARGS) // don't leak it
            return;
        if (input[i] == '"') {
            strflag = !strflag; // toggle string
        } else if (input[i] == ' ' && !strflag) {
            if (k > 0) { // ignore additional space
                args[arg_count++][k] = nullt; // insert null character
                k = 0;
            }
        } else {
            args[arg_count][k++] = input[i];
        }
        i++;
    }
    if (strflag)
        printChar('"'); // explicitly close the string
    if (k > 0 && args[arg_count][k-1] != nullt) { // insert null character
        args[arg_count++][k] = nullt;
    }
}

void shell() {
    // while (true){
    char input_buf[256];
    char path_str[256];
    while (true) {
        makeInterrupt21();
        printCWD();
        printStringColored(" >> ", COLOR_LIGHT_BLUE);
        readString(input_buf);
        printString(itoa(strlen(input_buf))); 
        parseArgs(input_buf);
        endl;
        if (arg_count > MAX_ARGS) {
            printString("Too many arguments"); endl;
            continue;
        } else if (arg_count == 0) {
            continue;
        }
        if (strcmp(args[0], "ls")) {
            if (arg_count > 1)
                ls(args[1]);
            else
                ls(0);
        } else if (strcmp(args[0], "mkdir")) {
            if (arg_count < 2) {
                printString("Too few arguments"); endl;
                continue;
            }
            mkdir(args[1]);
        } else if (strcmp(args[0], "cd")) {
            if (arg_count < 2) {
                printString("Too few arguments"); endl;
                continue;
            }
            cd(args[1]);
        } else {
            printString("Unknown command"); endl;
        }
    }

}
