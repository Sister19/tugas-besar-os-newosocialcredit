#include "../header/shell.h"

void ret_output(enum fs_retcode ret){
    switch (ret){
        case FS_SUCCESS:
            printString("Copy success!\n");
            break;
        case FS_W_NOT_ENOUGH_STORAGE:
            printStringColored("Error: Not enough storage to do copying!\n", COLOR_LIGHT_RED);
        // default:
        //     printString("Error\n");
        //     break;
    }
}

// copy file 1 (path1) to desired path (path2)
void cp(char* path1, char* path2){
    struct file_metadata metadata;
    enum fs_retcode ret;
    byte buffer[512 * 16];
    byte cdir1, ldir1, cdir2, ldir2;
    parsePathArg(path1);
    cdir1 = arg_cdir;
    ldir1 = arg_ldir;

    // check if path exists
    // "cat" implementation here, 
    if (checkIsExist(path1, cdir1) && checkIsFile(path1, cdir1)){
        metadata.parent_index = arg_ldir;
        metadata.buffer = buffer; // we put the buffer here
        strcpy(metadata.node_name, name_temp); // copy file name into metadata
        read(&metadata, &ret);

        parsePathArg(path2);
        cdir2 = arg_cdir;
        ldir2 = arg_ldir;

        // if the second path isn't a folder,
        // rename the file
        if (cdir2 == IDX_NODE_UNDEF){
            if (checkIsExist(path2, ldir2) && checkIsDirectory(path2, ldir2)){
                strcpy(metadata.node_name, name_temp);
                metadata.parent_index = ldir2;
                write(&metadata, &ret); 
                ret_output(ret);
            }
        } else {
            if (isDirectory(cdir2)){
                metadata.parent_index = cdir2;
                write(&metadata, &ret);
                ret_output(ret);
            } else {
                if (isDirectory(cdir1))
                    printStringColored("Can't copy directory into file.\n", COLOR_LIGHT_RED);
                else
                    printStringColored("Can't copy file into file.\n", COLOR_LIGHT_RED);
            }
        }
    
    }
}