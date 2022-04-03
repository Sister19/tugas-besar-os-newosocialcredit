#include "../header/utility.h"

void cd(char* path) {
    parsePathArg(path);
    if (checkIsExist(path, arg_cdir) && checkIsDirectory(path, arg_cdir))
        current_dir = arg_cdir;
}
