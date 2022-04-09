#include "../library/program.h"
void cpFile(byte from, byte to, char* last_name, char* current_name);

// copy file 1 (path1) to desired path (path2)
void main(){
    byte buffer[512 * 16];
    struct file_metadata metadata;
    enum fs_retcode ret;

    byte cdir1, ldir1, cdir2, ldir2;
    char* path1, *path2;
    char name_res[14];
    // always include these three lines (message passing).
    struct shell_data data;
    struct node_entry node;
    struct node_filesystem node_fs_buffer;

    getShellData(&data);
    if (data.cwd.arg_count > 2) {
        readsNode(&node_fs_buffer);
        metadata.buffer = buffer;
        path1 = data.arg.argv[1];
        path2 = data.arg.argv[2];
        parsePathArg(path1, data.cwd.current_dir, &cdir1, &ldir1, name_res, &node);
        // check if path exists
        // "cat" implementation here, 
        if (checkIsExist(path1, cdir1)){
            parsePathArg(path2, data.cwd.current_dir, &cdir2, &ldir2, name_res, &node);
            // if the second path is not found, use it as rename
            if (cdir2 == IDX_NODE_UNDEF){
                // and the second path is directory, copy folder
                if (checkIsExist(path2, ldir2) && checkIsDirectory(&node, path2, ldir2)){
                    metadata.parent_index = ldir1;
                    strncpy(metadata.node_name, node_fs_buffer.nodes[cdir1].name, 14);
                    read(&metadata, &ret);
                    metadata.parent_index = ldir2;
                    if (strlen(name_res) != 0)
                        strncpy(metadata.node_name, name_res, 14);
                    write(&metadata, &ret);

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
            } else {
                if (
                    isDirectory(&node, cdir2)
                    && getNodeIdx(&node_fs_buffer, node_fs_buffer.nodes[cdir1].name, cdir2) == IDX_NODE_UNDEF
                ) {
                    metadata.parent_index = ldir1;
                    strncpy(metadata.node_name, node_fs_buffer.nodes[cdir1].name, 14);
                    read(&metadata, &ret);
                    metadata.parent_index = cdir2;
                    write(&metadata, &ret);
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
                } else {
                    putsc("Can't copy file into file.\n", COLOR_LIGHT_RED);
                }
            }
        }
    }
    exit(&data);
}