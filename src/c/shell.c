#include "header/shell.h"

void get_dir_table(char* directory_table){

    // // calls interrupt to the handleInterrupt21 kernel function
    // // to execute the readSector function
    intr(0x21, 0x0002, directory_table, FS_NODE_SECTOR_NUMBER, 0);
    intr(0x21, 0x0002, directory_table + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1, 0);
    // // or alternatively call the function straight away?
    // readSector(directory_table, FS_NODE_SECTOR_NUMBER);
    // readSector(directory_table + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);
}

void getdir(char* string, char* directory_table, char curr_dir){
    char fname_buffer[16];
    char curr_parent;
    char parent_buf[FILE_ENTRY_COUNT];
    int i = 0;
    int parent_length = 0;

    // return "/" if the current directory is already in the root directory
    if (curr_dir == FS_NODE_P_IDX_ROOT){
        strcpy(string, "/");

    // else, find the directory
    } else {
        // if filled before, clear the parent buffer
        clear_char(parent_buf, FILE_ENTRY_COUNT);

        // fine the parent length
        curr_parent = directory_table[curr_dir * FILE_ENTRY_SIZE + PARENT_OFFSET];
        while (curr_parent != FS_NODE_P_IDX_ROOT){
            parent_buf[parent_length] = curr_parent;
            parent_length++;
            curr_parent = directory_table[curr_parent * FILE_ENTRY_SIZE + PARENT_OFFSET];
        }
        
        // create directory path with the string var
        i = parent_length - 1;

        while (i >= 0){
            strcat(string, "/");
            clear_char(fname_buffer, 16);
            strncpy(fname_buffer, directory_table + parent_buf[i] * FILE_ENTRY_SIZE + ENTRY_OFFSET, 14);
            strcat(string, fname_buffer);
            i--;
        }

        // add the latest directory name
        strcat(string, "/");
        clear_char(fname_buffer, 16);
        strncpy(fname_buffer, directory_table + curr_dir * FILE_ENTRY_SIZE + ENTRY_OFFSET, 14);
        strcat(string, fname_buffer);
    }
}

void mkdir(char* fname, byte current_dir){
    struct file_metadata metadata;
    enum fs_retcode ret;

    metadata.buffer = 0x00;
    strcpy(metadata.node_name, fname);
    metadata.parent_index = current_dir;
    metadata.filesize = 0;

    write(metadata, ret);
    switch (ret)
    {
        case 0:
            printString("Directory created successfully!"); endl;
            printString("Directory name: ");
            printString(&fname); endl;
            break;
        default:
            printString("Error something\r\n"); endl;
    }
    return;
}

void ls(char* directory_table, byte current_dir){
    char fname_buffer[16];
    int i = 0;
    char parent_entry_byte;
    char entry_entry_byte;
    printString("in LS"); endl;
    while (i < FILE_ENTRY_COUNT){
        parent_entry_byte = directory_table[i * FILE_ENTRY_SIZE + PARENT_OFFSET];
        entry_entry_byte = directory_table[i * FILE_ENTRY_SIZE + ENTRY_OFFSET];

        // inspeksi kalau parent entry byte nya sudah sesuai dengan current_dir
        // empty file entry kalau 0xFF - 0x1 (not demystified yet)
        if (parent_entry_byte == (char)current_dir && entry_entry_byte != 0xFE){
            clear_char(fname_buffer, sizeof(fname_buffer)); 
            strncpy(fname_buffer, directory_table + i * FILE_ENTRY_SIZE + ENTRY_OFFSET, 14);
            printString(fname_buffer); endl;
        }
        i++;
    }
    printString("out of LS"); endl;
}

void shell() {
    // while (true){
    char cmd_buf[10];
    char input_buf[256];
    char path_str[128];
    char string;
    char directory_table[SECTOR_SIZE * 2];
    byte current_dir = FS_NODE_P_IDX_ROOT;
    

    get_dir_table(directory_table);
    getdir(path_str, directory_table, current_dir);

    printString("(");
    printString(path_str);
    printString(") ");
    printString(path_str);
    printString("NewOS | >> ");  endl;



    mkdir("test", current_dir);
    printString("mkdir over"); endl;
    ls(directory_table, current_dir);

    printString("process over"); endl;
}
