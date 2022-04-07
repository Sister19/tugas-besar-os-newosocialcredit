#include "../header/utility.h"

enum fs_retcode createFolder(byte parent, char* name) {
    struct file_metadata metadata;
    enum fs_retcode ret;
    metadata.parent_index = parent;
    metadata.buffer = 0;
    strcpy(metadata.node_name, name);
    write(&metadata, &ret);
    return ret;
}

void mkdir(char* path){
    parsePathArg(path);
    if (checkIsExist(path, arg_ldir) && checkIsDirectory(path, arg_ldir)) {
        switch (createFolder(arg_ldir, name_temp))
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