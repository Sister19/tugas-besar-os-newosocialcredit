#include "program.h"
#include "screen.h"
#include "filesystem.h"

bool __init(char* name, int segment, byte cdir) {
    int i;
    struct file_metadata bin;
    enum fs_retcode ret;
    byte buffer[512 * 16];
    strncpy(bin.node_name, name, 14);
    bin.buffer = buffer;
    clear(buffer, 512*16);
    bin.parent_index = cdir; // we search for global bin (0x00) or local cwd
    read(&bin, &ret);
    if (ret == FS_SUCCESS) {
        for (i = 0; i < 8192; i++) {
            putInMemory(segment, i, buffer[i]);
        }
        return true;
    } else {
        return false;
    }
}

bool initProgram(struct parsed_arg *args, int segment) {
    bool res = __init(args->name_res, segment, args->arg_ldir);
    if (!res && !args->is_traversed) {
        return __init(args->name_res, segment, 0x0);
    }
    return res;
}

void getShellData(struct shell_data* data) {
    int i;
    readSector(&(data->cwd), 0x120);
    for (i = 0; i < 4; i++) {
        readSector(data->arg.argv[i], 0x121 + i);
    }
}

void setShellData(struct shell_data* data) {
    int i;
    writeSector(&(data->cwd), 0x120);
    for (i = 0; i < 4; i++) {
        writeSector(data->arg.argv[i], 0x121 + i);
    }
}
