#include "../library/program.h"
#include "../library/fileio.h"
#include "../library/textio.h"
#include "../library/args.h"

void main(){
    char* path;
    struct parsed_arg args;
    struct shell_data data;
    struct node_entry node;
    struct file_metadata metadata;
    enum fs_retcode ret;
    getShellData(&data);
    if (data.cwd.arg_count > 1) {
        path = data.arg.argv[1];
        parsePathArg(path, data.cwd.current_dir, &args);
        if (checkIsExist(path, args.arg_ldir) && checkIsDirectory(&node, path, args.arg_ldir)) {
            metadata.parent_index = args.arg_ldir;
            metadata.buffer = 0;
            strcpy(metadata.node_name, args.name_res);
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