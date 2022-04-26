#include "../library/program.h"
#include "../library/textio.h"
#include "../library/syscall.h"

void endGame(int len, struct shell_data* data);

int main() {
    struct shell_data data;
    int i, cur_idx = 0, dir = 0; // 0 = right / 1 = down / 2 = left / 3 = up
    int scancode, inp = 0, len = 1, cnt;
    int* curPos, *lastPos;
    int snakePos[100][2];
    int foodPos[2];
    getShellData(&data);
    for (i = 1; i < 100; i++) {
        snakePos[i][0] = -1;
        snakePos[i][1] = -1;
    }
    seed(getTime());
    snakePos[0][0] = mod(rand(), 80);
    snakePos[0][1] = mod(rand(), 24);
    foodPos[0] = mod(rand(), 80);
    foodPos[1] = mod(rand(), 24);
    while (true) {
        // clearScreen
        cls();
        if (snakePos[cur_idx][0] == foodPos[0] && snakePos[cur_idx][1] == foodPos[1]) {
            len++;
            foodPos[0] = mod(rand(), 80);
            foodPos[1] = mod(rand(), 24);
        }
        getb(&scancode, &inp);
        if (scancode == SC_CTRL && inp == 0x3) {
            exit(&data);
        }
        if (scancode == 72 && dir != 1) { //up
            dir = 3;
        } else if (scancode == 75 && dir != 0) { // left
            dir = 2;
        } else if (scancode == 77 && dir != 2) { // right
            dir = 0;
        } else if (scancode == 80 && dir != 3) { // down
            dir = 1;
        }
        lastPos = snakePos[cur_idx];
        cur_idx = mod((cur_idx + 1), 100);
        curPos = snakePos[cur_idx];
        curPos[0] = lastPos[0]; curPos[1] = lastPos[1];
        switch (dir) {
            case 0:
                ++(curPos[0]);
                break;
            case 1:
                ++(curPos[1]);
                break;
            case 2:
                --(curPos[0]);
                break;
            case 3:
                --(curPos[1]);
                break;
        }
        if (curPos[0] < 0) {
            curPos[0] = MAX_CURSOR_X;
        } else if (curPos[0] > MAX_CURSOR_X) {
            curPos[0] = 0;
        }
        if (curPos[1] < 0) {
            curPos[1] = MAX_CURSOR_Y;
        } else if (curPos[1] > MAX_CURSOR_Y) {
            curPos[1] = 0;
        }
        cnt = 0;
        if (len > 99) {
            setCurPos((80 - 24) / 2, (25 - 3) / 2);
            putsc("Max score limit reached!", COLOR_LIGHT_GREEN);
            endGame(len, &data);
        }
        for (i = 0; i < 100; i++) {
            if (curPos[0] == snakePos[i][0] && curPos[1] == snakePos[i][1] && ++cnt > 1) {
                setCurPos((80 - 9) / 2, (25 - 3) / 2);
                putsc("You lose!", COLOR_LIGHT_RED);
                endGame(len, &data);
            }
        }
        curPos = snakePos[mod((cur_idx - len), 100)];
        curPos[0] = -1;
        curPos[1] = -1;
        for (i = 0; i < 100; i++) {
            if (snakePos[i][0] != -1) {
                setCurPos(snakePos[i][0], snakePos[i][1]);
                putc(' ', 0xF0);
            }
        }
        setCurPos(foodPos[0], foodPos[1]);
        putc(' ', 0xC0);
        setCurPos(0, MAX_CURSOR_Y);
        puts("Score: "); putsc(itoa(len-1), COLOR_LIGHT_GREEN);
        setCurPos(MAX_CURSOR_X + 1, MAX_CURSOR_Y + 1);
        sleep(2);
    }
}

void endGame(int len, struct shell_data* data) {
    int scancode, inp;
    setCurPos((80 - 9) / 2, (25 - 3) / 2 + 1);
    puts("Score: "); putsc(itoa(len-1), COLOR_LIGHT_GREEN);
    setCurPos((80 - 25) / 2, (25 - 3) / 2 + 2);
    putsc("Try again (R) or exit (C)", COLOR_LIGHT_CYAN);
    setCurPos(MAX_CURSOR_X + 1, MAX_CURSOR_Y + 1);
    while (true) {
        get(&scancode, &inp);
        if (inp == 'R' || inp == 'r') {
            cls();
            exec(((data->cwd.prog_count-1) % 2) * 0x1000 + 0x3000);
        } else if (inp == 'C' || inp == 'c') {
            cls();
            exit(data);
        }
    }
}