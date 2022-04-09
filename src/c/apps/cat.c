#include "../library/program.h"
#include "../library/fileio.h"
#include "../library/textio.h"

void main() {
    int i, r;
    // reading file stuffs
    struct file_metadata metadata;
    enum fs_retcode ret;
    byte buffer[512*16];
    // argparse
    byte arg_cdir, arg_ldir;
    char* path;
    char name_res[14];
    // always include these three lines (message passing).
    struct shell_data data;
    struct node_entry node;
    getShellData(&data);
    if (data.cwd.arg_count > 1) {
        path = data.arg.argv[1];
        parsePathArg(path, data.cwd.current_dir, &arg_cdir, &arg_ldir, name_res, &node);
        if (checkIsExist(path, arg_cdir) && checkIsFile(&node, path, arg_cdir)) {
            metadata.parent_index = arg_ldir;
            metadata.buffer = buffer;
            strcpy(metadata.node_name, name_res);
            read(&metadata, &ret);
            switch (ret)
            {
                case FS_SUCCESS:
                    putl(metadata.buffer);
                    break;
                default:
                    putsc("Error reading file.\n", COLOR_LIGHT_RED);
                    break;
            }
        }
    } else {
        puts("Specify path file to read.\n");
        puts("Usage: cat <path_file>\n");
    }
    exit(&data);
}