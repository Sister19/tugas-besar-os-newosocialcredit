#include "../library/program.h"

void main(){
    byte arg_cdir, arg_ldir;
    char* path;
    char name_res[14];
    struct shell_data data;
    struct node_entry node;
    struct file_metadata metadata;
    enum fs_retcode ret;
    getShellData(&data);
    if (data.cwd.arg_count > 1) {
        path = data.arg.argv[1];
        parsePathArg(path, data.cwd.current_dir, &arg_cdir, &arg_ldir, name_res, &node);
        if (checkIsExist(path, arg_ldir) && checkIsDirectory(&node, path, arg_ldir)) {
            metadata.parent_index = arg_ldir;
            metadata.buffer = 0;
            strcpy(metadata.node_name, name_res);
            write(&metadata, &ret);
            switch (ret)
            {
                case FS_SUCCESS:
                    puts("Directory created successfully.\n");
                    break;
                case FS_W_FILE_ALREADY_EXIST:
                    putsc("Error: Folder has existed.\n", COLOR_LIGHT_RED);
                    break;
                default:
                    puts("Error: "); puts(itoa(ret)); endl;
                    break;
            }
        }
    }
    exit(&data);
}