#include "../library/program.h"
#include "../library/fileio.h"
#include "../library/textio.h"

void main(){
    byte arg_cdir, arg_ldir;
    int i;
    char* path;
    char name_res[14];
    struct shell_data data;
    struct node_entry node;
    struct file_metadata metadata;
    enum fs_retcode ret;
    getShellData(&data);
    if (data.cwd.arg_count > 1 && data.cwd.arg_count <= 5) {
        for(i=1; i<data.cwd.arg_count; i++){
            path = data.arg.argv[i];
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
    } else {
        puts("Specify directory name or path to make.\n");
        puts("Up to 4 directories can be made at once.\n");
        puts("Usage: mkdir <directory_name_1> <directory_name_2> ... <directory_name_4>\n");
    }
    exit(&data);
}