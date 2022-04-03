#include "../header/shell.h"
#include "../header/utility.h"
void mv(char* path1, char* path2) {
    byte cdir1, ldir1;
    parsePathArg(path1);
    cdir1 = arg_cdir; ldir1 = arg_ldir;
    if (checkIsExist(path1, cdir1)) { // make sure path1 exist
        parsePathArg(path2);
        if (arg_cdir == IDX_NODE_UNDEF) { // dest is not exist, use it as new name
            // last dest should exist and is directory
            if (checkIsExist(path2, arg_ldir) && checkIsDirectory(path2, arg_ldir)) {
                // we rename the folder.
                strcpy(node_fs_buffer.nodes[cdir1].name, name_temp);
                // we move it to new parent (arg_ldir)
                node_fs_buffer.nodes[cdir1].parent_node_index = arg_ldir;
                writeNodeFs(&node_fs_buffer);
            }
        } else { // dest is exist, we just move the directory.
            if (isDirectory(arg_cdir)) { 
                node_fs_buffer.nodes[cdir1].parent_node_index = arg_cdir;
                writeNodeFs(&node_fs_buffer);
            } else { // if dest is a file, we can't move it.
                if (isDirectory(cdir1))
                    printString("Can't move directory into file.\n");
                else
                    printString("Can't move file into file.\n");
            }
        }
    }
}