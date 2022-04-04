#include "../header/shell.h"

void ret_output(enum fs_retcode ret){
    switch (ret){
        case FS_SUCCESS:
            printString("Copy success.\n");
            break;
        case FS_W_NOT_ENOUGH_STORAGE:
            printStringColored("Error: Not enough storage to do copying.\n", COLOR_LIGHT_RED);
            break;
        default:
            printString("Error\n");
            break;
    }
}

enum fs_retcode cpFile(byte from, byte to, char* last_name, char* current_name) {
    byte buffer[512 * 16];
    struct file_metadata metadata;
    enum fs_retcode ret;
    metadata.parent_index = from;
    metadata.buffer = buffer;
    strcpy(metadata.node_name, last_name);
    read(&metadata, &ret); // asumsi selalu sukses
    metadata.parent_index = to;
    if (current_name != 0)
        strcpy(metadata.node_name, current_name);
    write(&metadata, &ret);
    return ret;
}

// copy file 1 (path1) to desired path (path2)
void cp(char* path1, char* path2){
    byte cdir1, ldir1, cdir2, ldir2;
    parsePathArg(path1);
    cdir1 = arg_cdir;
    ldir1 = arg_ldir;

    // check if path exists
    // "cat" implementation here, 
    if (checkIsExist(path1, cdir1)){
        parsePathArg(path2);
        cdir2 = arg_cdir;
        ldir2 = arg_ldir;
        // if the second path is not found, use it as rename
        if (cdir2 == IDX_NODE_UNDEF){
            // and the second path is directory, copy folder
            if (checkIsExist(path2, ldir2) && checkIsDirectory(path2, ldir2)){
                ret_output(
                    cpFile(
                        cdir1,
                        ldir2,
                        node_fs_buffer.nodes[cdir1].name,
                        name_temp
                    )
                );
            }
        } else {
            if (
                isDirectory(cdir2)
                && getNodeIdxFromParent(node_fs_buffer.nodes[cdir1].name, cdir2) == IDX_NODE_UNDEF
            ) {
                ret_output(
                    cpFile(
                        cdir1,
                        cdir2,
                        node_fs_buffer.nodes[cdir1].name,
                        0
                    )
                );
            } else {
                printStringColored("Can't copy file into file.\n", COLOR_LIGHT_RED);
            }
        }
    
    }
}