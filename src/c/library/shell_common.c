#include "shell_common.h"

void getShellData(struct shell_data* data) {
    intr(0x21, REG(0x9, 0x5), data, 0, 0);
}

void setShellData(struct shell_data* data) {
    intr(0x21, REG(0xA, 0x5), data, 0, 0);
}
