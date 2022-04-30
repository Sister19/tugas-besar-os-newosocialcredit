#include "../library/program.h"
#include "../library/textio.h"

int main() {
    struct shell_data data;
    getShellData(&data);
    putsc("====================================================", 0x0A); endl;
    putsc("||  _   _               _____ _____        _____  ||", 0x0A); endl;
    putsc("|| | \\ | |             |  _  /  ___|      /  __ \\ ||", 0x0A); endl;
    putsc("|| |  \\| | _____      _| | | \\ `--. ______| /  \\/ ||", 0x0A); endl;
    putsc("|| | . ` |/ _ \\ \\ /\\ / / | | |`--. \\______| |     ||", 0x0A); endl;
    putsc("|| | |\\  |  __/\\ V  V /\\ \\_/ /\\__/ /      | \\__/\\ ||", 0x0A); endl;
    putsc("|| \\_| \\_/\\___| \\_/\\_/  \\___/\\____/        \\____/ ||", 0x0A); endl;
    putsc("|| v.3.0.0                                        ||",  0x0A); endl;
    putsc("====================================================", 0x0A); endl;
    putsc("  Made by three dudes: ", 0x0A); endl;
    putsc("- 13520103 - Amar Fadil", 0x0A); endl;
    putsc("- 13520124 - Owen Christian Wijaya", 0x0A); endl;
    putsc("- 13520139 - Fachry Dennis Heraldi", 0x0A); endl;
    endl;endl;
    exit(&data);
}