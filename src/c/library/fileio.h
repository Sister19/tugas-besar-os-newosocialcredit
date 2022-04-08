#ifndef FILEIO__H
#define FILEIO__H

#include "../includes/struct_fs.h"

// 0x2
void readsNode(struct node_filesystem *node_fs_buffer);
void writesNode(struct node_filesystem *node_fs_buffer);
void reads(byte* buf, int sec_num);
void writes(byte* buf, int sec_num);

// 0x3
void read(struct file_metadata *metadata, enum fs_retcode *return_code);
void write(struct file_metadata *metadata, enum fs_retcode *return_code);
int getNodeIdx(struct node_filesystem *node_fs_buffer, char* name, byte parent);
void putInMemory(int segment, int address, byte b);

#endif