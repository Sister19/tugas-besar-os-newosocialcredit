#include "header/filesystem.h"
#include "header/interrupt.h"
#include "header/std_lib.h"

void readSector(byte* buffer, int sector_number){
    int sector_read_count = 0x01;
    int cylinder = (sector_number / SECTORS_PER_CYLINDER);
    int sector = mod(sector_number, SECTORS_PER_HEAD) + 1;
    int head = mod((sector_number / SECTORS_PER_HEAD), 2);
    int drive = 0x00;

    // call interrupt
    intr(
        INT_RW, // interrupt 0x13 untuk read/write sectors
        AX_READ, // 0x0201 untuk read
        buffer, // buffer
        REG(cylinder, sector), //CH untuk cylinder, CL untuk sektor
        REG(head, drive) // DH untuk head, DL untuk drive
    );
}

void writeSector(byte* buffer, int sector_number){
    int sector_read_count = 0x01;
    int cylinder = (sector_number / SECTORS_PER_CYLINDER);
    int sector = mod(sector_number, SECTORS_PER_HEAD) + 1;
    int head = mod((sector_number / SECTORS_PER_HEAD), 2);
    int drive = 0x00;
    
    // call interrupt
    intr(
        INT_RW, // interrupt 0x13 untuk read/write sectors
        AX_WRITE , // 0x0301 untuk write
        buffer, // buffer
        REG(cylinder, sector), //CH untuk cylinder, CL untuk sektor
        REG(head, drive) // DH untuk head, DL untuk drive
    );
}

void fillMap(){
    struct map_filesystem map_fs_buffer;
    readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
    // mengubah sektor 0-15 dan 256-511 menjadi terisi
    int i = 0;
    for (i = 0; i < 15; i++){
        map_fs_buffer.is_filled[i] = true;
    }
    for (i = 256; i < 511; i++){
        map_fs_buffer.is_filled[i] = true;
    }
    writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code){
    // return FS_R_TYPE_IS_FOLDER if destination is a folder
    // return FS_SUCCESS If destination is a file
    // filesize variable not used

    // metadata isinya bfufer, node_name, parent_size
    // enum retcode untuk return code
    // Tambahkan tipe data yang dibutuhkan
    struct node_filesystem node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct node_entry node_buffer;
    struct sector_entry sector_entry_buffer;
    // Masukkan filesystem dari storage ke memori buffer
    // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
    // Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
    // Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
    // dan keluar.
    // 2. Cek tipe node yang ditemukan
    // Jika tipe node adalah file, lakukan proses pembacaan.
    // Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
    // dan keluar.

    // membaca storage ke buffer
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);


    // Pembacaan
    // 1. memcpy() entry sector sesuai dengan byte S 
    // 2. Lakukan iterasi proses berikut, i = 0..15
    // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
    // 4. Jika byte bernilai 0, selesaikan iterasi
    // 5. Jika byte valid, lakukan readSector()
    // dan masukkan kedalam buffer yang disediakan pada metadata
    // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
    // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.
}