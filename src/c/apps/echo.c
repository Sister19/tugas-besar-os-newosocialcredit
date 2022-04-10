#include "../library/program.h"
#include "../library/textio.h"

int main() {
    struct shell_data data;
    int i;
    getShellData(&data);
    i = 5;
    while (data.cwd.input[i] != nullt) {
        if (data.cwd.input[i] != '"')
            put(data.cwd.input[i]);
        ++i;
    }
    endl;
    exit(&data);
}