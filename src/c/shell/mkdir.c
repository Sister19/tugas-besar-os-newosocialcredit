#include "../header/shell.h"

void mkdir(char* path){
    struct file_metadata metadata;
    enum fs_retcode ret;
    parsePathArg(path);
    if (checkIsExist(path, arg_ldir) && checkIsDirectory(path, arg_ldir)) {
        metadata.parent_index = arg_ldir;
        metadata.buffer = 0;
        strcpy(metadata.node_name, name_temp);
        write(&metadata, &ret);
        switch (ret)
        {
            case FS_SUCCESS:
                printString("Directory created successfully!"); endl;
                break;
            default:
                printString("Error"); endl;
                break;
        }
    }
}