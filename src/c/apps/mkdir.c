#include "../library/program.h"
#include "../library/fileio.h"
#include "../library/textio.h"
#include "../library/args.h"

void main(){
    int i;
    char* path;
    struct parsed_arg args;
    struct shell_data data;
    struct file_metadata metadata;
    enum fs_retcode ret;
    getShellData(&data);
    if (data.cwd.arg_count > 1 && data.cwd.arg_count <= 5) {
        for(i=1; i<data.cwd.arg_count; i++) {
            path = data.arg.argv[i];
            parsePathArg(path, data.cwd.current_dir, &args);
            if (checkIsExist(path, args.arg_ldir) && checkIsDirectory(args.node, path, args.arg_ldir)) {
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
    } else {
        puts("Specify directory name or path to make.\n");
        puts("Up to 4 directories can be made at once.\n");
        puts("Usage: mkdir <directory_name_1> <directory_name_2> ... <directory_name_4>\n");
    }
    exit(&data);
}