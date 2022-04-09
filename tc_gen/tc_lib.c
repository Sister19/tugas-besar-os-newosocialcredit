typedef unsigned char byte;
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

int getEmptyNodeIdx(byte* buf) {
    int i = 0;
    while (i < 64 && buf[i*16+2] != '\0')
        i++;
    return i;
}

// insert_file & create_folder menerima relative pathing
void insert_file(byte buf[2880][512], char *fname, byte parent_idx) {
    // load file
    FILE *file = fopen(fname, "rb");
    if (file == NULL) {
        printf("Error : \"%s\" not found\n", fname);
        return;
    }
    byte buffer[16][512];
    // get file length (seek end, tell, and rewind to beginning)
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    rewind(file);
    // read file to buffer and close
    fread(buffer, 512, 16, file);
    fclose(file);
    char name[14];
    strncpy(name, basename(fname), 14); // use its basename
    // offset filesystem
    byte* node_fs = buf[0x101];
    byte* map_fs = buf[0x100];
    byte* sector_fs = buf[0x103];
    // check if avail
    int idx = getEmptyNodeIdx(node_fs);
    if (idx == 64) {
        printf("Error inserting %s: not enough node\n", name);
        return;
    }
    // find empty sector
    int sec_idx = 0;
    while (sec_idx < 32 && sector_fs[sec_idx*16] != 0)
        sec_idx++;
    if (sec_idx == 32) {
        printf("Error inserting %s: sector entry is full\n", name);
        return;
    }
    // write file to empty sector
    int count = 0;
    int sec_num = 0;
    int sec_act_idx = 16; // we dont want to write kernel
    while (count < len) {
        while (sec_act_idx < 0x100 && map_fs[sec_act_idx])
            sec_act_idx++;
        if (sec_act_idx == 0x100) {
            printf("Error inserting %s: not enough sector\n", name);
            return;
        }
        map_fs[sec_act_idx] = 1;
        sector_fs[sec_idx * 16 + sec_num] = sec_act_idx;
        memcpy(&buf[sec_act_idx], &buffer[sec_num], 512);
        ++sec_num;
        count += 512;
    }
    // add file to node
    strncpy(&node_fs[idx * 16 + 2], name, 14);
    node_fs[idx * 16] = parent_idx;
    node_fs[idx * 16 + 1] = sec_idx;
    printf("Writer : Writing %s completed (%d bytes)\n", name, len);
}

int create_folder(byte buf[2880][512], char *fname, byte parent_idx) {
    // add folder
    byte* buf_node = buf[0x101];
    int idx = getEmptyNodeIdx(buf_node);
    if (idx == 64) {
        printf("Error creating %s: not enough node\n", fname);
        return 64;
    }
    strncpy(&buf_node[idx * 16 + 2], fname, 14);
    buf_node[idx*16] = parent_idx;
    buf_node[idx*16+1] = 0xFF;
    printf("Writer : Folder %s created\n", fname);
    return idx;
}