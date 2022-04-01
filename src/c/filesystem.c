#include "header/filesystem.h"
#include "header/screen.h"

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
    int i = 0;
    struct map_filesystem map_fs_buffer;
    readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
    // mengubah sektor 0-15 dan 256-511 menjadi terisi

    for (i = 0; i < 16; i++)
        map_fs_buffer.is_filled[i] = true;

    for (i = 256; i < 512; i++)
        map_fs_buffer.is_filled[i] = true;

    writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
}

void readNodeFs(struct node_filesystem *node_fs_buffer) {
    readSector(&(node_fs_buffer->nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer->nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}

byte getNodeIdxFromParent(struct node_filesystem *node_fs_buffer, char* name, byte parent) {
    byte i;
    for (i = 0; i < 64; i++) {
        if (
            node_fs_buffer->nodes[i].parent_node_index == parent
            && strcmp(node_fs_buffer->nodes[i].name, name)
        ) {
            return i;
        }
    }
    return 64;
}

byte getNodeIdx(struct node_filesystem *node_fs_buffer, char* name) {
    byte i;
    for (i = 0; i < 64; i++) {
        if (strcmp(node_fs_buffer->nodes[i].name, name)) {
            return i;
        }
    }
    return 64;
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code){
    // Tambahkan tipe data yang dibutuhkan
    struct node_filesystem   node_fs_buffer;
    struct node_entry        node_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct sector_entry      sector_entry_buffer;
    char file_partition[512];
    
    int i = 0;
    int j = 0;
    bool is_node_found = false;

    // Masukkan filesystem dari storage ke memori buffer
    readNodeFs(&node_fs_buffer);
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);

    
    // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
    // Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
    // Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
    // dan keluar.

    // Asumsi metadata sudah terdefinisi dengan benar (masukan dari parameter valid)
    i = 0;
    while (i < 64 && !is_node_found){
        if (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name)){ // Nama node cocok
            if (node_fs_buffer.nodes[i].parent_node_index == metadata->parent_index){ // Parent node cocok
                is_node_found = true;
            }
        }
        else {
            i++;
        }
    }
    
    if (!is_node_found){
        *return_code = FS_R_NODE_NOT_FOUND;
        return;
    }
    
    // 2. Cek tipe node yang ditemukan
    // Jika tipe node adalah file, lakukan proses pembacaan.
    // Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
    // dan keluar.
    if (node_fs_buffer.nodes[i].sector_entry_index != FS_NODE_S_IDX_FOLDER) { // Node bertipe file
        // Pembacaan
        // 1. memcpy() entry sector sesuai dengan byte S 
        memcpy(
            &sector_entry_buffer, // Menyimpan informasi entry sector dari node yang ditemukan
            &(sector_fs_buffer.sector_list[node_fs_buffer.nodes[i].sector_entry_index]),
            sizeof(struct sector_entry)
        );

        // 2. Lakukan iterasi proses berikut, i = 0..15
        // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
        // 4. Jika byte bernilai 0, selesaikan iterasi
        while (j < 16 && sector_entry_buffer.sector_numbers[j] != 0x00){
            clear(file_partition, 512);
            // 5. Jika byte valid, lakukan readSector()
            readSector(file_partition,sector_entry_buffer.sector_numbers[j]);
            // dan masukkan kedalam buffer yang disediakan pada metadata
            memcpy(
                &(metadata->buffer[j * 512]),
                file_partition,
                512
            );
            j++;
        } // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
        // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.
        *return_code = FS_SUCCESS;
        return;

    } else { // Node bertipe folder (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER)
        *return_code = FS_R_TYPE_IS_FOLDER;
        return;
    }
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
    char buffer[512];
    struct node_filesystem   node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct map_filesystem    map_fs_buffer;
    // Tambahkan tipe data yang dibutuhkan
    int index_node, index_map, index_sector;
    int count_empty_sector;
    int count_partition;
    int i,j;
    bool is_node_found;
    bool is_sector_found;
    bool is_write_complete;
    bool is_folder;
    
    // Masukkan filesystem dari storage ke memori
    readSector(&(map_fs_buffer.is_filled[0]), FS_MAP_SECTOR_NUMBER);
    readNodeFs(&node_fs_buffer);
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
    
    // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
    //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
    //    Jika ditemukan node yang cocok, tuliskan retcode 
    //    FS_W_FILE_ALREADY_EXIST dan keluar. 
    index_node = 0;
    is_node_found = false;
    while (index_node < 64 && !is_node_found){
        if (strcmp(node_fs_buffer.nodes[index_node].name, metadata->node_name)){ // Nama node cocok
            if (node_fs_buffer.nodes[index_node].parent_node_index == metadata->parent_index){ // Parent node cocok
                is_node_found = true;
            }
        }
        else {
            index_node++;
        }
    }

    if (is_node_found){
        *return_code = FS_W_FILE_ALREADY_EXIST;
        return;
    }
    
    // 2. Cari entri kosong pada filesystem node dan simpan indeks.
    //    Jika ada entry kosong, simpan indeks untuk penulisan.
    //    Jika tidak ada entry kosong, tuliskan FS_W_MAXIMUM_NODE_ENTRY
    //    dan keluar.
    index_node = 0;
    is_node_found = false;
    while (index_node < 64 && !is_node_found){ // Node yang kosong adalah jika nama node kosong 
        if (strlen(node_fs_buffer.nodes[index_node].name) == 0){ // Nama node kosong
            is_node_found = true; // Node kosong ditemukan, informasi indeksnya adalah index_node
        }
        else {
            index_node++;
        }
    }

    if (!is_node_found){
        *return_code = FS_W_MAXIMUM_NODE_ENTRY;
        return;
    }
    

    // 3. Cek dan pastikan entry node pada indeks P adalah folder.
    //    Jika pada indeks tersebut adalah file atau entri kosong,
    //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.
    if (node_fs_buffer.nodes[metadata->parent_index].sector_entry_index != FS_NODE_S_IDX_FOLDER){
        // *return_code = FS_W_INVALID_FOLDER;
        *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
    }


    // 4. Dengan informasi metadata filesize, hitung sektor-sektor 
    //    yang masih kosong pada filesystem map. Setiap byte map mewakili 
    //    satu sektor sehingga setiap byte mewakili 512 bytes pada storage.
    //    Jika empty space tidak memenuhi, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika ukuran filesize melebihi 8192 bytes, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika tersedia empty space, lanjutkan langkah ke-5.
    count_empty_sector = 0;
    for (index_map = 0; index_map < 256; index_map++){ // Pengecekan empty space 
        if (!map_fs_buffer.is_filled[index_map]){
            count_empty_sector++;
        }
    }
    if (count_empty_sector < metadata->filesize / 512){
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    }
    if (metadata->filesize > 8192){
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    }

    // 5. Cek pada filesystem sector apakah terdapat entry yang masih kosong.
    //    Jika ada entry kosong dan akan menulis file, simpan indeks untuk 
    //    penulisan.
    //    Jika tidak ada entry kosong dan akan menulis file, tuliskan
    //    FS_W_MAXIMUM_SECTOR_ENTRY dan keluar.
    //    Selain kondisi diatas, lanjutkan ke proses penulisan.
    index_sector = 0;
    is_sector_found = false;
    while (index_sector < 32 && !is_sector_found){
        if (sector_fs_buffer.sector_list[index_sector].sector_numbers[0] == 0){ 
            is_sector_found = true; // Sector kosong ditemukan, informasi indeksnya adalah index_sector
        }
        else {
            index_sector++;
        }
    }
    if (!is_sector_found){
        *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
    }

    // Penulisan
    // 1. Tuliskan metadata nama dan byte P ke node pada memori buffer
    strcpy(node_fs_buffer.nodes[index_node].name, metadata->node_name);
    node_fs_buffer.nodes[index_node].parent_node_index = metadata->parent_index;
    
    // 2. Jika menulis folder, tuliskan byte S dengan nilai 
    //    FS_NODE_S_IDX_FOLDER dan lompat ke langkah ke-8
    // 3. Jika menulis file, tuliskan juga byte S sesuai indeks sector
    is_folder = false;
    if (metadata->filesize == 0){
        node_fs_buffer.nodes[index_node].sector_entry_index = FS_NODE_S_IDX_FOLDER;
        is_folder = true;
    }
    else {
        node_fs_buffer.nodes[index_node].sector_entry_index = index_sector;
    }

    if (!is_folder){
        // 4. Persiapkan variabel j = 0 untuk iterator entry sector yang kosong
        j = 0;
        // 5. Persiapkan variabel buffer untuk entry sector kosong

        count_partition=0;
        is_write_complete = false;
        i = 0;
        
        // 6. Lakukan iterasi berikut dengan kondisi perulangan (penulisan belum selesai && i = 0..255)
        while (i < 256 && !is_write_complete){
            //    1. Cek apakah map[i] telah terisi atau tidak
            //    2. Jika terisi, lanjutkan ke iterasi selanjutnya / continue
            if(!map_fs_buffer.is_filled[i]){
                //    3. Tandai map[i] terisi
                map_fs_buffer.is_filled[i] = true;
                //    4. Ubah byte ke-j buffer entri sector dengan i
                sector_fs_buffer.sector_list[index_sector].sector_numbers[j] = i;
                //    5. Tambah nilai j dengan 1
                j++;
                //    6. Lakukan writeSector() dengan file pointer buffer pada metadata 
                //       dan sektor tujuan i
                clear(buffer[512], 512);
                memcpy(buffer, metadata->buffer+count_partition*512, 512);
                writeSector(buffer, i);
                count_partition++;
                //    7. Jika ukuran file yang telah tertulis lebih besar atau sama dengan
                //       filesize pada metadata, penulisan selesai
                if (count_partition*512 >= metadata->filesize){
                    is_write_complete = true;
                }
            } else {
                i++;
            }
        }
    
        // 7. Lakukan update dengan memcpy() buffer entri sector dengan 
        //    buffer filesystem sector
        // Tidak perlu karena sudah dilakukan perubahan pada langkah 6.4
    }
    
    // 8. Lakukan penulisan seluruh filesystem (map, node, sector) ke storage
    //    menggunakan writeSector() pada sektor yang sesuai
    writeSector(&(map_fs_buffer.is_filled[0]), FS_MAP_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    writeSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);

    // 9. Kembalikan retcode FS_SUCCESS
    *return_code = FS_SUCCESS;
    
}
