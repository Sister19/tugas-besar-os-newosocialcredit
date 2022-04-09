#include "../library/program.h"
#include "../includes/struct_fs.h"
#include "../library/textio.h"

void main() {
    // usage for parsing args.
    byte arg_cdir, arg_ldir;
    char* path;
    char name_res[14];
    // always include these three lines (message passing).
    struct shell_data data;
    struct node_entry node;
    getShellData(&data);
    // Check argument count. (arguments is always parsed, so don't call parseArgs again.)
    if (data.cwd.arg_count > 1) {
        path = data.arg.argv[1]; // path is in second argument (first arg is program name)
        parsePathArg(path, data.cwd.current_dir, &arg_cdir, &arg_ldir, name_res, &node);
        if (checkIsExist(path, arg_cdir) && checkIsDirectory(&node, path, arg_cdir)) {
            data.cwd.current_dir = arg_cdir;
            setShellData(&data);
        }
    } else {
        puts("Specify path directory destination.\n");
        puts("Usage: cd <path_dir>\n");
    }
    exit(&data);
}
