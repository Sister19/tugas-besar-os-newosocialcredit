#include "../library/program.h"
#include "../includes/struct_fs.h"
#include "../library/textio.h"
#include "../library/args.h"

void main() {
    int i = 0, cnt = 0;
    char *path;
    struct shell_data data;
    struct node_filesystem node_fs_buffer;
    struct parsed_arg args;
    struct node_entry* node;
    getShellData(&data); readsNode(&node_fs_buffer);
    path = data.arg.argv[1];
    if (data.cwd.arg_count > 1) {
        parsePathArg(path, data.cwd.current_dir, &args);
        if (!(checkIsExist(path, args.arg_cdir) && checkIsDirectory(args.node, path, args.arg_cdir)))
            exit(&data);
    } else { // either way, we set current to current_dir
        args.arg_cdir = data.cwd.current_dir;
    }
    while (i < 64) {
        *node = node_fs_buffer.nodes[i];
        if (node->name[0] != nullt && node->parent_node_index == args.arg_cdir) {
            if (node->sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                putsc(node->name, COLOR_LIGHT_GREEN); sp; sp;
            } else {
                puts(node->name); sp; sp;
            }
            ++cnt;
        }
        ++i;
    }
    if (cnt > 0){
        endl;
    } else {
        putsc("Directory empty.", COLOR_LIGHT_RED); endl;
    }
    exit(&data);
}