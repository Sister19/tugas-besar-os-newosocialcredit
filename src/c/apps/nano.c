#include "../library/args.h"
#include "../library/program.h"
#include "../library/textio.h"
#include "../library/syscall.h"
#include "../library/fileio.h"
#include "../library/string.h"
#define IS_PRINTABLE(input) (input >= 0x20 && input <= 0x7e)

char buffer[8192];
int start, index, x, y, len, end;

void indexGo(bool forward);
void cursorGo(bool forward);
void scroll(bool up);

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
        end = start;
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
            ++end;
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
                cursorGo(false);
            }
            else if (scancode == SC_RARROW && index < len) {
                cursorGo(true);
            }
            else if (scancode == SC_UARROW) {
                if (y == 1)
                    // scroll down
                    scroll(false);
                else
                    indexGo(false);
            }
            else if (scancode == SC_DARROW) {
                if (y == 23)
                    // scroll up
                    scroll(true);
                else
                    indexGo(true);
            }
            else if (input == KEY_BKSP && index > 0) {
                for (i = --index; i < len - 1; i++)
                    buffer[i] = buffer[i + 1];
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

void cursorGo(bool forward) {
    int i, j, len;
    if (forward && ((index < 8192 && buffer[index] == '\n') || x == MAX_CURSOR_X)) {
        x = 0;
        if (y == 23) {
            // scroll up
            scroll(true);
        } else {
            ++y;
        }
    } else if (!forward && x == 0) {
        if (y == 1) {
            //scroll down
            scroll(false);
        } else {
            --y;
        }
        i = index - 2;
        while (i >= 0 && buffer[i] != '\n') {
            ++x; --i;
            if (x > MAX_CURSOR_X) x = 0;
        }
    } else {
        if (forward) ++x;
        else --x;
    }
    index += forward ? 1 : -1;
}

void scroll(bool up) {
    int i, j;
    if (up) {
        // geser keatas
        if (end > len - 1) return;
        indexGo(true);
        y = MAX_CURSOR_Y - 1;
        i = start;
        j = x;
        while (buffer[i] != nullt) {
            if (buffer[i] == '\n' || j > MAX_CURSOR_X)
                break;
            ++i;
            ++j;
        }
        start = ++i;
    } else {
        // geser kebawah
        //WIP
        if (start == 0) return;
        --start;
        j = 0;
        while (start > 0) {
            if (buffer[start] == '\n' || j > MAX_CURSOR_X)
                break;
            --start;
            ++j;
        }
        indexGo(false);
        y = 1;
    }
}

void indexGo(bool forward) {
    int i, j, k;
    if (forward) {
        k = x;
        i = index;
        j = x;
        while (i < len && buffer[i] != '\n' && j < MAX_CURSOR_X) {
            ++i; ++j;
        }
        if (buffer[i] == '\n') ++i;
        index = i;
        if (j >= MAX_CURSOR_X) x = -1; else x = 0;
        ++y;
        while (x < k && buffer[index] != '\n') {
            ++index; ++x;
        }
    } else {
        i = x;
        index -= x; x = 0;
        if (buffer[index] == '\n') --index;
        cursorGo(false);
        if (i < x) {
            index -= x-i;
            x = i;
        }
    }
}
