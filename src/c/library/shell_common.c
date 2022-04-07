#include "shell_common.h"
#include "syscall.h"

void spawnShell() {
    exec(0x2000);
}

void getShellData(struct shell_data* data) {
    int i;
    readSector(&(data->cwd), 0x120);
    for (i = 0; i < 4; i++) {
        readSector(data->arg.argv + 512 * i, 0x121 + i);
        readSector(&(data->hist) + 512 * i, 0x125 + i);
    }
}

void setShellData(struct shell_data* data) {
    int i;
    writeSector(&(data->cwd), 0x120);
    for (i = 0; i < 4; i++) {
        writeSector(data->arg.argv + 512 * i, 0x121 + i);
        writeSector(&(data->hist) + 512 * i, 0x125 + i);
    }
}
