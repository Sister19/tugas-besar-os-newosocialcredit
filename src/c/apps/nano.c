#include "../library/args.h"
#include "../library/program.h"
#include "../library/textio.h"
#include "../library/syscall.h"
#include "../library/fileio.h"
#include "../library/string.h"
#define IS_PRINTABLE(input) (input >= 0x20 && input <= 0x7e)

char buffer[8192];
int start, index, x, y, len;

void setXY(int _y, bool forward, bool keepX);

int main() {
    int i, j, k, lenTitle = 13, lines;
    int scancode, input;
    char* title = "  New  File  ";
    char* path;
    bool isDirty = false;
    bool isSaving = false;
    char filename[14];
    struct shell_data data;
    struct parsed_arg args;
    struct file_metadata file;
    enum fs_retcode ret;
    getShellData(&data);
    clear(buffer, 8192);
    file.buffer = buffer;
    file.parent_index = data.cwd.current_dir;
    if (data.cwd.arg_count > 1) {
        path = data.arg.argv[1];
        parsePathArg(path, data.cwd.current_dir, &args);
        if (args.arg_cdir == IDX_NODE_UNDEF) { // dest not exist, use it as new name
            if (checkIsExist(path, args.arg_ldir) && checkIsDirectory(args.node, path, args.arg_ldir)) {
                file.parent_index = args.arg_ldir;
                strncpy(file.node_name, args.name_res, 14);
            } else {
                exit(&data);
            }
        } else {
            if (checkIsFile(args.node, path, args.arg_cdir)) {
                lenTitle = strlen(args.name_res);
                title = args.name_res;
                file.parent_index = args.arg_ldir;
                strncpy(file.node_name, args.name_res, 14);
                read(&file, &ret);
                if (ret != 0) {
                    putsc("Can't read the file contents!\n", COLOR_LIGHT_RED);
                    exit(&data);
                }
                strncpy(file.node_name, args.name_res, 14);
            } else {
                exit(&data);
            }
        }
    }
    x = 0; y = 1;
    len = strlen(buffer);
    index = 0;
    start = 0;
    while (true) {
        cls(); // always refresh screen
        // print top
        for (i = 0; i < 80; i++) {
            if (i == (80 - lenTitle) / 2) {
                putsc(title, 0xF0);
                i += lenTitle - 1;
            } else
                putc(' ', 0xF0);
        }
        // print text
        lines = 1;
        i = start;
        j = 0;
        while (buffer[i] != nullt) {
            if (buffer[i] == '\n' || j > MAX_CURSOR_X) {
                j = -1; // compensate for last ++j and print ('\n')
                ++lines;
            }
            if (lines > 23)
                break;
            put(buffer[i]);
            ++j;
            ++i;
        }
        // print bottom
        if (isSaving) {
            setCurPos(0, 24);
            putsc("^C", 0xF0); puts(" Cancel");
            setCurPos(0, 23);
            putsc("File Name to Write:", 0xF0); sp;
            getsn(file.node_name, 14);
            if (strlen(file.node_name)) {
                strncpy(title, file.node_name, 14);
                lenTitle = strlen(title);
                write(&file, &ret);
                isDirty = false;
            }
            isSaving = false;
        } else {
            setCurPos(0, 24);
            putsc("^X", 0xF0); puts(" Exit    ");
            if (isDirty) {
                putsc("^O", 0xF0); puts(" Save");
            }
            setCurPos(x, y);
            get(&scancode, &input);
            if (scancode == SC_LARROW && index > 0) {
                setXY(y, false, false);
            }
            else if (scancode == SC_RARROW && index < len) {
                setXY(y, true, false);
            }
            else if (scancode == SC_UARROW) {
                setXY(y - 1, false, true);
                // index -= x; // skip till last newline
                // i = index;
                // j = MAX_CURSOR_X;
                // while (i >= 0) {
                //     if (buffer[i] == '\n' || j < 0)
                //         break;
                //     --i;
                //     --j;
                // }
                // j = MAX_CURSOR_X - j; // dapetin x
                // if (x > j) x = j;
                // --y;
                // index += x;
            }
            else if (scancode == SC_DARROW) {
                setXY(y + 1, true, true);
                // i = index;
                // j = x;
                // while (buffer[i] != nullt) {
                //     if (buffer[i] == '\n' || j > MAX_CURSOR_X)
                //         break;
                //     ++i;
                //     ++j;
                // }
                // index += j - x; // skip till newline
                // j = 0;
                // while (buffer[i] != nullt) {
                //     if (buffer[i] == '\n' || j > MAX_CURSOR_X)
                //         break;
                //     ++i;
                //     ++j;
                // }
                // if (x > j) x = j;
                // ++y;
                // index += x;
            }
            else if (input == KEY_BKSP && index > 0) {
                for (i = --index; i < len - 1; i++)
                {
                    buffer[i] = buffer[i + 1];
                }
                buffer[--len] = nullt;
                moveCursor(true);
                getCurPos(&x, &y);
            }
            else if (IS_PRINTABLE(input) && len < 8191) {
                for (i = len++; i > index; i--) {
                    buffer[i] = buffer[i - 1];
                }
                buffer[index++] = input;
                moveCursor(false);
                getCurPos(&x, &y);
                isDirty = true;
            } else if (scancode == 45 && input == 24) { // ctrl + x
                cls();
                exit(&data);
            } else if (scancode == 24 && input == 15) { // ctrl + o
                if (isDirty) {
                    isSaving = true;
                }
            }
        }
    }
}

// int getIndex(int x, int y) {
//     int i, j, len, lines;
//     i = start;
//     j = 0;
//     len = 0;
//     lines = 0;
//     while (buffer[i] != nullt) {
//         if (buffer[i] == '\n' || j > MAX_CURSOR_X) {
//             len = j;
//             j = 0;
//             lines++;
//         }
//         if (lines == y)
//             break;
//         ++j;
//         ++i;
//     }
//     if (buffer[i] == nullt)
//         len = j;
//     return len;
// }

// void getPos(int index, int* x, int* y) {
//     int i;
//     i = start;
//     *x = 0;
//     *y = 1;
//     while(i < 8192 && i != index) {
//         if (buffer[i] == '\n' || *x > MAX_CURSOR_X) {
//             *x = -1; // compensate for x++
//             ++(*y);
//         }
//         ++(*x);
//         ++i;
//     }
// }

int getLen(int _y) {
    int i, j, len, lines;
    i = start;
    j = 0;
    len = 0;
    lines = 0;
    while (buffer[i] != nullt) {
        if (buffer[i] == '\n' || j > MAX_CURSOR_X) {
            len = j;
            j = 0;
            lines++;
        }
        if (lines == _y)
            break;
        ++j;
        ++i;
    }
    if (buffer[i] == nullt)
        len = j;
    return len;
}

int calculateDelta(int _x, int _y, bool forward) {
    int i = index, cnt = 0;
    if (forward) {
        while (i < len) {
            if (x == _x && y == _y) break;
            if (buffer[i] == '\n' || _x > MAX_CURSOR_X) {
                _x = 0;
                ++y;
            }
            ++cnt;
            ++i;
        }
    } else {
        while (i >= 0) {
            if (x == _x && y == _y) break;
            if (buffer[i] == '\n' || _x < 0) {
                _x = MAX_CURSOR_X;
                --y;
            }
            --cnt;
            --i;
        }
    }
    return cnt;
}

void setXY(int _y, bool forward, bool keepX) {
    int i, j, k, l;
    int _x = x + (keepX ? 0 : (forward ? 1: -1));
    // update index in x (after update y if any)
    if (_x < 0) { // mentok ke kiri
        --_y;
        _x = getLen(_y);
    } else {
        l = getLen(_y);
        if (_x > l) { // mentok ke kanan
            ++_y;
            _x = 0;
        }
    }
    // update start
    if (_y < 1) {
        // geser kebawah kalau masih ada
        _y = 1;
        if (start > 0) {
            i = start - 1;
            j = x;
            while (i > 0) {
                if (buffer[i] == '\n' || j < 0)
                    break;
                --i;
                --j;
            }
            start = --i;
        }
    } else if (_y > MAX_CURSOR_Y - 1) {
        // geser keatas
        _y = MAX_CURSOR_Y - 1;
        i = start;
        j = x;
        while (buffer[i] != nullt) {
            if (buffer[i] == '\n' || j > MAX_CURSOR_X)
                break;
            ++i;
            ++j;
        }
        if (buffer[++i] != nullt)
            start = i;
    }
    index += calculateDelta(_x, _y, forward);
    // int i, j, idx;
    // int i, j, k, lines, l;
    // if (_x < 0) {
    //     --_y;
    //     l = getLen(_y);
    //     if (keepX) {
    //         if (_x > l) _x = l;
    //         index -= l - _x;
    //     } else {
    //         _x = l;
    //     }
    // } else {
    //     l = getLen(_y);
    //     if (_x > l) {
    //         ++_y;
    //         index += _x - l;
    //         if (keepX) {
    //             l = getLen(_y);
    //             if (_x > l) _x = l;
    //             index += _x;
    //         } else {
    //             _x = 0;
    //         }
    //     }
    // }
    // index += _x - x;
    // if (y < _y) {
    //     index += getLen(y);
    // } else if (y > _y) {
    //     index -= getLen(y);
    // }
    // // if (y < _y) { // kebawah

    // // } else if (y > _y) { // keatas
    // //     i = index;
    // //     j = x;
    // //     lines = 0;
    // //     while (buffer[i] != nullt) {
    // //         if (buffer[i] == '\n' || j > MAX_CURSOR_X){
    // //             ++lines;
    // //             len = j;
    // //             j = 0;
    // //         }
    // //         if (lines < _y - y)
    // //             break;
    // //         ++i;
    // //         ++j;
    // //     }
    // // }
    // // // determine _x and index
    // // if (y < _y) { // mau kebawah
    // //     i = index;
    // //     j = x;
    // //     while (buffer[i] != nullt) {
    // //         if (buffer[i] == '\n' || j > MAX_CURSOR_X)
    // //             break;
    // //         ++i;
    // //         ++j;
    // //     }
    // //     // find length
    // //     k = i++;
    // //     j = 0;
    // //     while (buffer[i] != nullt) {
    // //         if (buffer[i] == '\n' || j > MAX_CURSOR_X)
    // //             break;
    // //         ++i;
    // //         ++j;
    // //     }
    // //     len = i - k;
    // //     _x = x > len ? len : x;
    // //     index += k + _x - 1;
    // // } else if (y > _y) { // mau keatas
    // //     i = index;
    // //     j = x;
    // //     while (i >= 0) {
    // //         if (buffer[i] == '\n' || j < 0)
    // //             break;
    // //         --j;
    // //         --i;
    // //     }
    // //     // find length
    // //     k = i--;
    // //     j = MAX_CURSOR_X;
    // //     while (buffer[i] != nullt) {
    // //         if (buffer[i] == '\n' || j < 0)
    // //             break;
    // //         --j;
    // //         --i;
    // //     }
    // //     len = k - i;
    // //     _x = x > len ? len : x;
    // //     index -= k + _x - 1;
    // // } else {
    // //     index += _x - x;
    // // }
    // // scrolling upmost/downmost
    // if (_y < 1) {
    //     // geser kebawah
    //     _y = 1;
    //     i = start;
    //     j = x;
    //     while (i >= 0) {
    //         if (buffer[i] == '\n' || j > MAX_CURSOR_X)
    //             break;
    //         --i;
    //         ++j;
    //     }
    //     start = --i;
    // } else if (_y > MAX_CURSOR_Y - 1) {
    //     // geser keatas
    //     _y = MAX_CURSOR_Y - 1;
    //     i = start;
    //     j = x;
    //     while (buffer[i] != nullt) {
    //         if (buffer[i] == '\n' || j > MAX_CURSOR_X)
    //             break;
    //         ++i;
    //         ++j;
    //     }
    //     start = ++i;
    // }
    x = _x; y = _y;
}