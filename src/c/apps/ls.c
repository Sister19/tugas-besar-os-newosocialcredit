#include "../library/program.h"

void main() {
    int i = 0, cnt = 0;
    byte arg_cdir, arg_ldir;
    char name_res[14];
    char *path;
    struct shell_data data;
    struct node_filesystem node_fs_buffer;
    struct node_entry node;
    getShellData(&data); readsNode(&node_fs_buffer);
    path = data.arg.argv[1];
    if (data.cwd.arg_count > 1) {
        parsePathArg(path, data.cwd.current_dir, &arg_cdir, &arg_ldir, name_res, &node);
        if (!(checkIsExist(path, arg_cdir) && checkIsDirectory(&node, path, arg_cdir)))
            exit(&data);
    } else { // either way, we set current to current_dir
        arg_cdir = data.cwd.current_dir;
    }
    while (i < 64) {
        node = node_fs_buffer.nodes[i];
        if (node.name[0] != nullt && node.parent_node_index == arg_cdir) {
            if (node.sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                putsc(node.name, COLOR_LIGHT_GREEN); sp; sp;
            } else {
                puts(node.name); sp; sp;
            }
            cnt++;
        }
        i++;
    }
    if (cnt > 0){
        endl;
    } else {
        putsc("Directory empty.", COLOR_LIGHT_RED); endl;
    }
    exit(&data);
}