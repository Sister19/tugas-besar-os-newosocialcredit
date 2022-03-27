#include "header/shell.h"

void get_dir_table(char* directory_table){

    // // calls interrupt to the handleInterrupt21 kernel function
    // // to execute the readSector function
    // intr(0x21, 0x2, directory_table, FS_NODE_SECTOR_NUMBER, 0);
    // intr(0x21, 0x2, directory_table + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1, 0);
    // or alternatively call the function straight away?
    readSector(directory_table, FS_NODE_SECTOR_NUMBER);
    readSector(directory_table + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);
}

void getdir(char* string, char* directory_table, char curr_dir){
    char fname_buffer[16];
    char curr_parent;
    char parent[FILE_ENTRY_COUNT];
    int i = 0;
    int parent_length = 0;

    // return "/" if the current directory is already in the root directory
    if (curr_dir == FS_NODE_P_IDX_ROOT){
        strcpy(string, "welcome to root (this the printcwd part)");

    // else, find the directory
    } else {
        // if filled before, clear the parent buffer
        clear_char(parent, FILE_ENTRY_COUNT);

        // fine the parent length
        curr_parent = directory_table[curr_dir * FILE_ENTRY_SIZE + PARENT_OFFSET];
        while (curr_parent != FS_NODE_P_IDX_ROOT){
            parent[parent_length] = curr_parent;
            curr_parent = directory_table[curr_parent * FILE_ENTRY_SIZE + PARENT_OFFSET];
            parent_length++;
        }
        
        // create directory path with the string var
        i = parent_length - 1;
        while (i >= 0){
            strcat(string, "/");
            clear_char(fname_buffer, 16);
            strncpy(fname_buffer, directory_table + parent[i] * FILE_ENTRY_SIZE + ENTRY_OFFSET, FNAME_LEN);
            strcat(string, fname_buffer);
        }

        // add the latest directory name
        strcat(string, "/");
        clear_char(fname_buffer, 16);
        strncpy(fname_buffer, directory_table + curr_dir * FILE_ENTRY_SIZE + ENTRY_OFFSET, FNAME_LEN);
        strcat(string, fname_buffer);
    }
}

void shell() {
    char input_buf[64];
    char path_str[128];
    char string;
    char directory_table[SECTOR_SIZE * 2];
    byte current_dir = FS_NODE_P_IDX_ROOT;
    

    printString("OS@IF2230:");

    get_dir_table(directory_table);
    getdir(path_str, directory_table, current_dir);
    // printCWD(path_str, current_dir);
    printString("$");
    printString(path_str);
    
    // readString(input_buf);
    // if (strcmp(input_buf, "cd")) {
    // // Utility cd
    // }
    // else{
    //     printString("Unknown command\r\n");
    // }
}

// void ls(char* dir){
//     int i = 0;
//     char fname_buffer[16];
// }