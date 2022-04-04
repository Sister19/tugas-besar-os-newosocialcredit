#include "header/syscall.h"
#include "header/constant.h"

// 0x0
void printLines(char* buf) {
    intr(0x21, 0x00, buf, 0, 1);
}
void printStringColored(char *buf, int col) {
    intr(0x21, 0x00, buf, col, 2);
}
void printString(char *buf){
    intr(0x21, 0x00, buf, 0, 3);
};

void printChar(char c){
    intr(0x21, 0x00, c, 0, 4);
};

void printCharColored(char c, char color){
    intr(0x21, 0x00, c, color, 5);
};
// 0x1
void readString(char *buf){
    intr(0x21, 0x1, buf, 0, 0);
};
void readKey(char* scancode, char *key){
    intr(0x21, 0x1, scancode, key, 1);
};
// 0x2
void readNodeFs(struct node_filesystem *node_fs_buffer) {
    readSector(node_fs_buffer->nodes, FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer->nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}

void writeNodeFs(struct node_filesystem *node_fs_buffer) {
    writeSector(node_fs_buffer->nodes, FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer->nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}

void readSector(byte* buf, int sec_num){
    intr(0x21, 0x2, buf, sec_num, 0);
};

void writeSector(byte* buf, int sec_num){
    intr(0x21, 0x2, buf, sec_num, 1);
};;
// 0x3
void read(struct file_metadata *metadata, enum fs_retcode *return_code){
    intr(0x21, 0x3, metadata, return_code, 0);
};

void write(struct file_metadata *metadata, enum fs_retcode *return_code){
    intr(0x21, 0x3, metadata, return_code, 1);
};

int getNodeIdxFromParent(char* name, unsigned char parent){
    return intr(0x21, 0x3, name, &parent, 2);
}
// 0x4
void clearScreen(){
    intr(0x21, 0x4, 0, 0, 0);
}