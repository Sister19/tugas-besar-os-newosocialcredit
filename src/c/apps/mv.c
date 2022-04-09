#include "../library/program.h"
#include "../library/fileio.h"
#include "../library/textio.h"

void main() {
    byte cdir1, ldir1, arg_cdir, arg_ldir;
    char name_res[14];
    char *path1, *path2;
    struct shell_data data;
    struct node_filesystem node_fs_buffer;
    struct node_entry node;
    getShellData(&data);
    if (data.cwd.arg_count > 2) {
        path1 = data.arg.argv[1];
        path2 = data.arg.argv[2];
        parsePathArg(path1, data.cwd.current_dir, &cdir1, &ldir1, name_res, &node);
        readsNode(&node_fs_buffer);
        if (checkIsExist(path1, cdir1)) { // make sure path1 exist
            parsePathArg(path2, data.cwd.current_dir, &arg_cdir, &arg_ldir, name_res, &node);
            if (arg_cdir == IDX_NODE_UNDEF) { // dest is not exist, use it as new name
                // last dest should exist and is directory
                if (checkIsExist(path2, arg_ldir) && checkIsDirectory(&node, path2, arg_ldir)) {
                    // we rename the folder.
                    strcpy(node_fs_buffer.nodes[cdir1].name, name_res);
                    // we move it to new parent (arg_ldir)
                    node_fs_buffer.nodes[cdir1].parent_node_index = arg_ldir;
                    writesNode(&node_fs_buffer);
                    puts("Move success.\n");
                }
            } else { // dest is exist, we just move the directory.
                if (
                    isDirectory(&node, arg_cdir)
                    && getNodeIdx(&node_fs_buffer, node_fs_buffer.nodes[cdir1].name, arg_cdir) == IDX_NODE_UNDEF
                ) { 
                    node_fs_buffer.nodes[cdir1].parent_node_index = arg_cdir;
                    writesNode(&node_fs_buffer);
                    puts("Move success.\n");
                } else { // if dest is a file, we can't move it.
                    if (isDirectory(&node_fs_buffer.nodes[cdir1], cdir1))
                        puts("Can't move directory into file.\n");
                    else
                        puts("Can't move file into file.\n");
                }
            }
        }
    }
    exit(&data);
}