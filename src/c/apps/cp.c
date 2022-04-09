#include "../library/program.h"
#include "../library/fileio.h"
#include "../library/textio.h"
#include "../library/args.h"
#include "../library/std_lib.h"

void cpFile(byte from, byte to, char* last_name, char* current_name);

// copy file 1 (path1) to desired path (path2)
void main(){
    struct parsed_arg args1;
    struct parsed_arg args2;
    char* path1, *path2;
    // always include these three lines (message passing).
    struct shell_data data;
    struct node_filesystem node_fs_buffer;

    getShellData(&data);
    if (data.cwd.arg_count > 2) {
        readsNode(&node_fs_buffer);
        path1 = data.arg.argv[1];
        path2 = data.arg.argv[2];
        parsePathArg(path1, data.cwd.current_dir, &args1);
        // check if path exists
        // "cat" implementation here, 
        if (checkIsExist(path1, args1.arg_cdir)){
            parsePathArg(path2, data.cwd.current_dir, &args2);
            // if the second path is not found, use it as rename
            if (args2.arg_cdir == IDX_NODE_UNDEF){
                // and the second path is directory, copy folder
                if (checkIsExist(path2, args2.arg_ldir) && checkIsDirectory(args2.node, path2, args2.arg_ldir)){
                    cpFile(
                        args1.arg_ldir,
                        args2.arg_ldir,
                        node_fs_buffer.nodes[args1.arg_cdir].name,
                        args2.name_res
                    );
                }
            } else {
                if (
                    isDirectory(args2.node, args2.arg_cdir)
                    && getNodeIdx(&node_fs_buffer, node_fs_buffer.nodes[args1.arg_cdir].name, args2.arg_cdir) == IDX_NODE_UNDEF
                ) {
                    cpFile(
                        args1.arg_ldir,
                        args2.arg_cdir,
                        node_fs_buffer.nodes[args1.arg_cdir].name,
                        0
                    );
                } else {
                    putsc("Can't copy file into file.\n", COLOR_LIGHT_RED);
                }
            }
        }
    }
    exit(&data);
}

void cpFile(byte from, byte to, char* last_name, char* current_name) {
    byte buffer[512 * 16];
    struct file_metadata metadata;
    enum fs_retcode ret;
    metadata.parent_index = from;
    metadata.buffer = buffer;
    strncpy(metadata.node_name, last_name, 14);
    read(&metadata, &ret);
    if (ret == 0) {
        metadata.parent_index = to;
        if (current_name != 0)
            strncpy(metadata.node_name, current_name, 14);
        write(&metadata, &ret);
    }
    switch (ret){
        case FS_SUCCESS:
            puts("Copy success.\n");
            break;
        case FS_W_NOT_ENOUGH_STORAGE:
            putsc("Error: Not enough storage.\n", COLOR_LIGHT_RED);
            break;
        default:
            puts("Error "); puts(itoa(ret)); endl;
            break;
    }
}
