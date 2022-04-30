#include "../library/program.h"
#include "../library/textio.h"
#include "../library/string.h"

int main() {
    struct shell_data data;
    bool is_valid = false;
    getShellData(&data);
    puts("i "); putsc("love", COLOR_LIGHT_RED); puts(" you, so "); putsc("much.", COLOR_LIGHT_GREEN); endl;
    puts("please be my "); putsc("angel", COLOR_LIGHT_MAGENTA); puts(", "); putsc("baby.", COLOR_LIGHT_BLUE); endl;
    while (!is_valid) {
        puts("your response: (yes/no) "); gets(data.cwd.input); endl;
        is_valid = strcmp(data.cwd.input, "yes") || strcmp(data.cwd.input, "no");
        if (is_valid) {
            if (strcmp(data.cwd.input,"yes")) {
                puts("thank you "); putsc("<3", COLOR_LIGHT_MAGENTA); endl;
            } else {
                putsc("asge.", COLOR_LIGHT_RED); endl;
            }
        } else {
            putsc("invalid response.", COLOR_LIGHT_RED); endl;
        }
    } 
    exit(&data);
}