#include "../header/utility.h"

void cat(char* path) {
    struct file_metadata metadata;
    enum fs_retcode ret;
    byte buffer[512*16];
    int i, r;
    parsePathArg(path);
    if (checkIsExist(path, arg_cdir) && checkIsFile(path, arg_cdir)) {
        metadata.parent_index = arg_ldir;
        metadata.buffer = buffer;
        strcpy(metadata.node_name, name_temp);
        read(&metadata, &ret);
        switch (ret)
        {
            case FS_SUCCESS:
                printLines(metadata.buffer);
                break;
            default:
                printStringColored("Error reading file.\n", COLOR_LIGHT_RED);
                break;
        }
    }
}