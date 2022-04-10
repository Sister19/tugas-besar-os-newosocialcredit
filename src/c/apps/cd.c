#include "../library/program.h"
#include "../includes/struct_fs.h"
#include "../library/textio.h"
#include "../library/args.h"

void main() {
    // usage for parsing args.
    struct parsed_arg args;
    char* path;
    // always include these three lines (message passing).
    struct shell_data data;
    getShellData(&data);
    // Check argument count. (arguments is always parsed, so don't call parseArgs again.)
    if (data.cwd.arg_count > 1) {
        path = data.arg.argv[1]; // path is in second argument (first arg is program name)
        parsePathArg(path, data.cwd.current_dir, &args);
        if (checkIsExist(path, args.arg_cdir) && checkIsDirectory(args.node, path, args.arg_cdir)) {
            data.cwd.current_dir = args.arg_cdir;
            setShellData(&data);
        }
    } else {
        puts("Specify path directory destination.\n");
        puts("Usage: cd <path_dir>\n");
    }
    exit(&data);
}
