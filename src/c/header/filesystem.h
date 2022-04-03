#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "interrupt.h"
#include "std_lib.h"
#include "std_type.h"
#include "struct_fs.h"

// header untuk fungsi
void fillMap();
void readSector(byte *buffer, int sector_number);
void readNodeFs();
void writeNodeFs();
void writeSector(byte* buffer, int sector_number);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);
void write(struct file_metadata *metadata, enum fs_retcode *return_code);
byte getNodeIdxFromParent(char* name, byte parent);

#endif