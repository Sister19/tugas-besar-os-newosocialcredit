#include "textio.h"
#include "../system/keyboard.h"

// 0x1
void gets(char *buf){
    intr(0x21, 0x1, buf, 0, 0);
};

void get(char* scancode, char *key){
    intr(0x21, REG(0x1, 0x1), scancode, key, 0);
};

// 0x0
void putsc(char *buf, int col) {
    intr(0x21, 0x00, buf, col, 0);
}
void puts(char *buf){
    intr(0x21, REG(0x1, 0x00), buf, 0, 0);
};

void put(char c){
    intr(0x21, REG(0x2, 0x00), c, 0, 0);
};

void putc(char c, char color){
    intr(0x21, REG(0x3, 0x00), c, color, 0);
};

void putl(char *buffer) {
    int input, scancode, i;
    char* enter = "Press ENTER to next or CTRL+C to exit...";
    bool is_stop = false;
    i = 0;
    while (buffer[i]) {
        if (buffer[i] == '\n') {
            endl;
            putsc(enter, 0xF0);
            while(true) {
                get(&scancode, &input);
                if (input == KEY_CR || input == KEY_LF) {
                    break;
                } else if (scancode == SC_CTRL && input == 0x3) {
                    is_stop = true;
                    break;
                }
            }
            input = 0;
            while (enter[input++]) {
                put(KEY_BKSP);
            }
            if (is_stop)
                return;
        } else {
            put(buffer[i]);
        }
        i++;
    }
    endl;
}
