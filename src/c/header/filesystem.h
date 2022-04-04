#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "interrupt.h"
#include "std_lib.h"
#include "std_type.h"

// Menunjukkan lokasi sektor filesystem
#define FS_MAP_SECTOR_NUMBER    0x100
#define FS_NODE_SECTOR_NUMBER   0x101
#define FS_SECTOR_SECTOR_NUMBER 0x103

// Isi byte P/S node yang terletak pada root.
#define FS_NODE_P_IDX_ROOT   0xFF
#define FS_NODE_S_IDX_FOLDER 0xFF
#define PARENT_OFFSET 0x0
#define ENTRY_OFFSET 0x1
#define FNAME_LEN 0xD
#define IDX_NODE_UNDEF 64

// read/write constants
#define SECTOR_SIZE 512
#define SECTORS_PER_CYLINDER 36
#define SECTORS_PER_HEAD 18
#define FILE_SECTOR_SIZE 2
#define FILE_ENTRY_SIZE 0x10 //16 byte per one entry in sector
#define FILE_ENTRY_COUNT (SECTOR_SIZE * FILE_SECTOR_SIZE / FILE_ENTRY_SIZE)
// Untuk filesystem map

struct map_filesystem {
    bool is_filled[512];
};

// Untuk filesystem nodes
struct node_entry {
    byte parent_node_index;
    byte sector_entry_index;
    char name[14];
};

struct node_filesystem {
    struct node_entry nodes[64];
};


// Untuk filesystem sector
struct sector_entry {
    byte sector_numbers[16];
};

struct sector_filesystem {
    struct sector_entry sector_list[32];
};


// Struktur data untuk read / write
struct file_metadata {
    byte* buffer;
    char node_name[14];
    byte parent_index;
    unsigned int filesize;
};

// Untuk error code write & read
enum fs_retcode {
    FS_UNKNOWN_ERROR    = -1,
    FS_SUCCESS          = 0,
    FS_R_NODE_NOT_FOUND = 1,
    FS_R_TYPE_IS_FOLDER = 2,

    FS_W_FILE_ALREADY_EXIST   = 3,
    FS_W_NOT_ENOUGH_STORAGE   = 4,
    FS_W_MAXIMUM_NODE_ENTRY   = 5,
    FS_W_MAXIMUM_SECTOR_ENTRY = 6,
    FS_W_INVALID_FOLDER       = 7
};

extern struct node_filesystem node_fs_buffer;
extern struct map_filesystem map_fs_buffer;
extern struct sector_filesystem sector_fs_buffer;

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