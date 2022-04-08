typedef unsigned char byte;
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "../includes/struct_fs.h"
#define NAMA_IMAGE "out/system.img"

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
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    rewind(file);
    fread(buffer, 512, 16, file);
    fclose(file);
    char name[14];
    strncpy(name, basename(fname), 14); // use its basename
    // offset filesystem
    byte* node_fs = buf[0x101];
    byte* map_fs = buf[0x100];
    byte* sector_fs = buf[0x103];
    // find empty sector
    int sec_idx = 0;
    while (sec_idx < 32 && sector_fs[sec_idx*16] != 0)
        sec_idx++;
    // write file to empty sector
    int count = 0;
    int sec_num = 0;
    int sec_act_idx = 16; // we dont want to write kernel
    while (count < len) {
        while (map_fs[sec_act_idx])
            sec_act_idx++;
        map_fs[sec_act_idx] = true;
        sector_fs[sec_idx * 16 + sec_num] = sec_act_idx;
        memcpy(&buf[sec_act_idx], &buffer[sec_num], 512);
        ++sec_num;
        count += 512;
    }
    // add file to node
    int idx = getEmptyNodeIdx(node_fs);
    strncpy(&node_fs[idx * 16 + 2], name, 14);
    node_fs[idx * 16] = parent_idx;
    node_fs[idx * 16 + 1] = sec_idx;
    printf("Writer : Writing %s completed (%d bytes)\n", name, len);
}

void create_folder(byte buf[2880][512], char *fname, byte parent_idx) {
    // add folder
    byte* buf_node = buf[0x101];
    int idx = getEmptyNodeIdx(buf_node);
    strncpy(&buf_node[idx * 16 + 2], fname, 14);
    buf_node[idx*16] = parent_idx;
    buf_node[idx*16+1] = FS_NODE_S_IDX_FOLDER;
    printf("Writer : Folder %s created\n", fname);
}

const int COUNT_APPS = 7;
char* apps[] = {
    "shell",
    "cat",
    "cd",
    "cp",
    "ls",
    "mkdir",
    "mv",
};

int main(int argc, char const *argv[]) {
    // Load entire file and save to buffer
    FILE *image = fopen(NAMA_IMAGE, "rb");

    if (image == NULL) {
        fprintf(stderr, "Error : File image <%s> not found\n", NAMA_IMAGE);
        exit(1);
    }
    printf("> Injecting apps to image...\n");
    byte imagebuffer[2880][512];
    for (int i = 0; i < 2880; i++)
        fread(imagebuffer[i], 512, 1, image);

    // Close file descriptor and overwrite
    fclose(image);
    image = fopen(NAMA_IMAGE, "wb");

    create_folder(imagebuffer, "bin", 0xFF);
    char fn[20];
    for (int i = 0; i < COUNT_APPS; i++) {
        strcpy(fn, "out/apps/");
        insert_file(imagebuffer, strcat(fn, apps[i]), 0);
    }

    // Overwrite old file
    for (int i = 0; i < 2880; i++)
        fwrite(imagebuffer[i], 512, 1, image);

    fclose(image);
    printf("> Apps successfully injected to image!\n");
    return 0;
}
