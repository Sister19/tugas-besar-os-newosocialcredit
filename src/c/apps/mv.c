#include "../library/program.h"
#include "../library/fileio.h"
#include "../library/textio.h"
#include "../library/args.h"

void main() {
    struct parsed_arg args1;
    struct parsed_arg args2;
    char *path1, *path2;
    struct shell_data data;
    struct node_filesystem node_fs_buffer;
    getShellData(&data);
    if (data.cwd.arg_count > 2) {
        path1 = data.arg.argv[1];
        path2 = data.arg.argv[2];
        parsePathArg(path1, data.cwd.current_dir, &args1);
        readsNode(&node_fs_buffer);
        if (checkIsExist(path1, args1.arg_cdir)) { // make sure path1 exist
            parsePathArg(path2, data.cwd.current_dir, &args2);
            if (args2.arg_cdir == IDX_NODE_UNDEF) { // dest is not exist, use it as new name
                // last dest should exist and is directory
                if (checkIsExist(path2, args2.arg_ldir) && checkIsDirectory(args2.node, path2, args2.arg_ldir)) {
                    // we rename the folder.
                    strcpy(node_fs_buffer.nodes[args1.arg_cdir].name, args2.name_res);
                    // we move it to new parent (arg_ldir)
                    node_fs_buffer.nodes[args1.arg_cdir].parent_node_index = args2.arg_ldir;
                    writesNode(&node_fs_buffer);
                    puts("Move success.\n");
                }
            } else { // dest is exist, we just move the directory.
                if (
                    isDirectory(args2.node, args2.arg_cdir)
                    && getNodeIdx(&node_fs_buffer, node_fs_buffer.nodes[args1.arg_cdir].name, args2.arg_cdir) == IDX_NODE_UNDEF
                ) { 
                    node_fs_buffer.nodes[args1.arg_cdir].parent_node_index = args2.arg_cdir;
                    writesNode(&node_fs_buffer);
                    puts("Move success.\n");
                } else { // if dest is a file, we can't move it.
                    if (isDirectory(&node_fs_buffer.nodes[args1.arg_cdir], args1.arg_cdir))
                        puts("Can't move directory into file.\n");
                    else
                        puts("Can't move file into file.\n");
                }
            }
        }
    }
    exit(&data);
}