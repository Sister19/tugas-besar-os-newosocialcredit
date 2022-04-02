#include "../header/shell.h"

void ret_output(enum fs_retcode ret){
    switch (ret){
        case FS_SUCCESS:
            printString("Copy success!\n");
            break;
        default:
            printString("Error in copying! :(\n");
            break;
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

        /*
        1. Bikin duplikat di cwd sekarang, terus ganti parent idx nya
        (mirip sama mv)
        2. parent index si metadata diubah jadi punya si path2, terus baru write
        */
        parsePathArg(path2);
        cdir2 = arg_cdir;
        ldir2 = arg_ldir;
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
                    printString("Can't copy directory into file.\n");
                else
                    printString("Can't copy file into file.\n");
            }
        }
        // assuming here we already have the data, we write the data back
    }
}