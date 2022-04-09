#include "args.h"
#include "textio.h"
#include "shell_common.h"
#include "fileio.h"

bool isDirectory(struct node_entry *node, byte cdir) {
    if (
        cdir == FS_NODE_P_IDX_ROOT || // is a root or a directory
        node->sector_entry_index == FS_NODE_S_IDX_FOLDER
    )
        return true;
    return false;
}

bool checkIsFile(struct node_entry *node, char* path, byte cdir) {
    if (isDirectory(node, cdir)) {
        putsc("Not a file: ", COLOR_LIGHT_RED);
        putsc(path, COLOR_LIGHT_BLUE); endl; // is not a file
        return false;
    }
    return true;
}

bool checkIsDirectory(struct node_entry *node, char* path, byte cdir) {
    if (!isDirectory(node, cdir)) {
        putsc("Not a directory: ", COLOR_LIGHT_RED);
        putsc(path, COLOR_LIGHT_BLUE); endl; // is not a directory
        return false;
    }
    return true;
}

bool checkIsExist(char* path, byte cdir) {
    if (cdir == IDX_NODE_UNDEF) {
        putsc("No such file or directory: ", COLOR_LIGHT_RED);
        putsc(path, COLOR_LIGHT_BLUE); endl; // no such file or directory
        return false;
    }
    return true;
}

void __applyPath(struct node_filesystem* node_fs_buffer, int dot_cnt, int len, struct parsed_arg *res) {
    if (res->arg_cdir == IDX_NODE_UNDEF) { // if cdir already undef, last set to undef
        res->arg_ldir = IDX_NODE_UNDEF;
    } else if (dot_cnt == 2 && len == 2) { // go up
        if (res->arg_cdir != FS_NODE_P_IDX_ROOT) { // if not root, we can go up
            res->arg_ldir = res->arg_cdir;
            res->arg_cdir = node_fs_buffer->nodes[res->arg_cdir].parent_node_index;
        }
    } else if (len > 0 && !(len == 1 && res->name_res[0] == '.')) { // at least len > 0 and not .
        res->name_res[len] = nullt;
        res->arg_ldir = res->arg_cdir;
        res->arg_cdir = getNodeIdx(node_fs_buffer, res->name_res, res->arg_cdir);
    }
}

void parsePathArg(char* path, byte current_dir, struct parsed_arg *res) {
    struct node_filesystem node_fs_buffer;
    byte dot_cnt = 0;
    int i = 0, len = 0;
    readsNode(&node_fs_buffer);
    res->arg_cdir = current_dir;
    res->arg_ldir = current_dir;
    if (path[i] == '/' || path[i] == '\\') {
        res->arg_cdir = FS_NODE_P_IDX_ROOT;
        res->arg_ldir = res->arg_cdir;
        ++i;
    }
        puts("a");
    while (path[i] != nullt) {
        if (path[i] == '.') ++dot_cnt;
        if (path[i] == '/' || path[i] == '\\') { // start to parse a new dir
            __applyPath(&node_fs_buffer, dot_cnt, len, res);
            len = 0;
            dot_cnt = 0;
        } else { // add to input
            if (len < 13)
                res->name_res[len++] = path[i];
        }
        i++;
    }
    
    puts("a");
    if (len > 0) // remains.
        __applyPath(&node_fs_buffer, dot_cnt, len, res);
    if (res->arg_cdir != IDX_NODE_UNDEF) {
        if (res->arg_cdir != FS_NODE_P_IDX_ROOT)
            *(res->node) = node_fs_buffer.nodes[res->arg_cdir];
    } else if(res->arg_ldir != IDX_NODE_UNDEF) {
        if (res->arg_ldir != FS_NODE_P_IDX_ROOT)
            *(res->node) = node_fs_buffer.nodes[res->arg_ldir];
    }
}

void parseArgs(struct shell_data *data) {
    int i = 0, k = 0;
    bool strflag = false;
    char* input = data->cwd.input;
    i = data->cwd.cur_inp_idx;
    data->cwd.arg_count = 0;
    data->cwd.parse_ret = INPUT_END;
    while (input[i] != nullt) {
        if (data->cwd.arg_count > MAX_ARGS) { // don't leak it
            data->cwd.parse_ret = ARG_LIMIT;
            return;
        }
        if (input[i] == '"')
            strflag = !strflag;
        else if (input[i] == ';') {
            while (input[++i] == ' '); // remove trail space if any
            if (input[i] != nullt)
                data->cwd.parse_ret = INPUT_TRAIL;
            break;
        } else if (input[i] == ' ' && !strflag) {
            if (k > 0) {
                data->arg.argv[data->cwd.arg_count][k] = nullt;
                ++data->cwd.arg_count;
                k = 0;
            }
        } else {
            data->arg.argv[data->cwd.arg_count][k++] = input[i];
        }
        ++i;
    }
    data->cwd.cur_inp_idx = i;
    if (strflag) {
        data->cwd.parse_ret = UNCLOSED_STRING;
        return;
    }
    if (k > 0 && data->arg.argv[data->cwd.arg_count][k-1] != nullt) { // insert null character
        data->arg.argv[data->cwd.arg_count][k] = nullt;
        ++data->cwd.arg_count;
    }
}
