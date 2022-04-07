#include "../header/utility.h"

void ls(char* path){
    int i = 0, cnt = 0;
    if (path != 0) {
        parsePathArg(path);
        if (!(checkIsExist(path, arg_cdir) && checkIsDirectory(path, arg_cdir)))
            return;
    } else { // either way, we also read fs node.
        readNodeFs(&node_fs_buffer);
        arg_cdir = current_dir;
    }
    while (i < 64) {
        struct node_entry node = node_fs_buffer.nodes[i];
        if (node.name[0] != nullt && node.parent_node_index == arg_cdir) {
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
    } else {
        printStringColored("Directory empty.", COLOR_LIGHT_RED); endl;
    }
}