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
                printString("Directory created successfully.\n");
                break;
            case FS_W_FILE_ALREADY_EXIST:
                printStringColored("Error: A folder with the same name has existed.\n", COLOR_LIGHT_RED);
                break;
            default:
                printString("Unknown error.\n");
                break;
        }
    }
}