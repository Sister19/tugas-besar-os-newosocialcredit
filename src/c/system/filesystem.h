#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../includes/interrupt.h"
#include "../library/std_lib.h"
#include "../library/string.h"
#include "../includes/struct_fs.h"

// header untuk fungsi
void fillMap();
void readSector(byte *buffer, int sector_number);
void readNodeFs(struct node_filesystem* node_fs_buffer);
void writeNodeFs(struct node_filesystem* node_fs_buffer);
void writeSector(byte* buffer, int sector_number);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);
void write(struct file_metadata *metadata, enum fs_retcode *return_code);
byte getNodeIdxFromParent(struct node_filesystem* node_fs_buffer, char* name, byte parent);

#endif