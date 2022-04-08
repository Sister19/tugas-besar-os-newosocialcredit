#include "fileio.h"
#include "../includes/interrupt.h"

// 0x2
void readsNode(struct node_filesystem *node_fs_buffer) {
    reads(node_fs_buffer->nodes, FS_NODE_SECTOR_NUMBER);
    reads(&(node_fs_buffer->nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}

void writesNode(struct node_filesystem *node_fs_buffer) {
    writes(node_fs_buffer->nodes, FS_NODE_SECTOR_NUMBER);
    writes(&(node_fs_buffer->nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}

void reads(byte* buf, int sec_num){
    intr(0x21, 0x2, buf, sec_num, 0);
};

void writes(byte* buf, int sec_num){
    intr(0x21, REG(0x1, 0x2), buf, sec_num, 0);
};;
// 0x3
void read(struct file_metadata *metadata, enum fs_retcode *return_code){
    intr(0x21, 0x3, metadata, return_code, 0);
}
void write(struct file_metadata *metadata, enum fs_retcode *return_code){
    intr(0x21, REG(0x1, 0x3), metadata, return_code, 1);
}
void putInMemory(int segment, int address, byte b) {
    intr(0x21, REG(0x2, 0x3), segment, address, b);
}
int getNodeIdx(struct node_filesystem *node_fs_buffer, char* name, byte parent){
    return intr(0x21, REG(0x3, 0x3), node_fs_buffer, name, parent);
}