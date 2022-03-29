#include "header/shell.h"

void get_dir_table(char* directory_table){

    // // calls interrupt to the handleInterrupt21 kernel function
    // // to execute the readSector function
    intr(0x21, 0x2, directory_table, FS_NODE_SECTOR_NUMBER, 0);
    intr(0x21, 0x2, directory_table + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1, 0);
    // or alternatively call the function straight away?
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
            curr_parent = directory_table[curr_parent * FILE_ENTRY_SIZE + PARENT_OFFSET];
            parent_length++;
        }
        
        // create directory path with the string var
        i = parent_length - 1;
        while (i >= 0){
            strcat(string, "/");
            clear_char(fname_buffer, 16);
            strncpy(fname_buffer, directory_table + parent_buf[i] * FILE_ENTRY_SIZE + ENTRY_OFFSET, FNAME_LEN);
            strcat(string, fname_buffer);
            i--;
        }

        // add the latest directory name
        strcat(string, "/");
        clear_char(fname_buffer, 16);
        strncpy(fname_buffer, directory_table + curr_dir * FILE_ENTRY_SIZE + ENTRY_OFFSET, FNAME_LEN);
        strcat(string, fname_buffer);
    }
}

void mkdir(char* fname, byte current_dir){
    struct file_metadata metadata;
    enum fs_retcode ret;

    metadata.buffer = 0x0;
    strcpy(metadata.node_name, fname);
    metadata.parent_index = current_dir;
    metadata.filesize = 0;

    write(metadata, ret);
    switch (ret){
        case 0:
            printString("Directory created successfully!\n");
            break;
        default:
            printString("Error something\n");
    }
}

void ls(char* directory_table, byte current_dir){
    char fname_buffer[16];
    int i = 0;
    char parent_entry_byte;
    char entry_entry_byte;
    while (i < FILE_ENTRY_COUNT){
        parent_entry_byte = directory_table[i * FILE_ENTRY_SIZE + PARENT_OFFSET];
        entry_entry_byte = directory_table[i * FILE_ENTRY_SIZE + ENTRY_OFFSET];

        // inspeksi kalau parent entry byte nya sudah sesuai dengan current_dir
        // empty file entry kalau 0xFF - 0x1 (not demystified yet)
        if (parent_entry_byte == current_dir && entry_entry_byte != 0xFF - 0x1){
            clear_char(fname_buffer, sizeof(fname_buffer)); 
            strcpy(fname_buffer, directory_table + i * FILE_ENTRY_SIZE + ENTRY_OFFSET);
            printString(fname_buffer); endl;
        }
    }
    i++;
    endl;
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
    // printCWD(path_str, current_dir);
    printString(path_str); endl;
    printString("NewOS | >> ");
    // readString(cmd_buf);
    mkdir("test", current_dir);
    ls(directory_table, current_dir);
    printString("\n");
    // if (strcmp(cmd_buf, "mkdir")) {
    //     printString("Input directory name: ");
    //     readString(input_buf);
    //     endl;
    //     mkdir(input_buf, current_dir);
    // }
    // else if (strcmp(cmd_buf, "ls")){
    //     ls(directory_table, current_dir);
    // }
    // else if (strcmp(cmd_buf, "exit")){
    //     return;
    // }
    // else{
    //     printString("Unknown command\r\n");
    // }
    // }
}
