#include "../library/program.h"
#include "../library/textio.h"

int main() {
    struct shell_data data;
    getShellData(&data);
    endl;
    putsc("cd <DEST>", COLOR_LIGHT_CYAN);
    puts("- Moves current working directory to destination path"); endl;
    putsc("ls", COLOR_LIGHT_CYAN);
    puts(" - Displays content of current directory"); endl;
    putsc("mv <FILE> <DEST_PATH>", COLOR_LIGHT_CYAN);
    puts(" - Moves a file to destination path"); endl;
    putsc("mkdir <DIR_NAME>", COLOR_LIGHT_CYAN);
    puts(" - Creates a new directory in current working directory"); endl;
    putsc("cat <FILE>", COLOR_LIGHT_CYAN);
    puts(" - Prints the content of a file"); endl;
    putsc("cp <FILE> <DEST_PATH> / <NEW_NAME>", COLOR_LIGHT_CYAN);
    puts(" - Copies a file to a path or to the same path with different name"); endl;
    endl;
    exit(&data);
}