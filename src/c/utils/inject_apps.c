typedef unsigned char byte;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/constant.h"
#define NAMA_IMAGE "out/system.img"

extern void insert_file(byte buf[2880][512], char *fname, byte parent_idx);
extern int create_folder(byte buf[2880][512], char *fname, byte parent_idx);

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

    int a = create_folder(imagebuffer, "bin", 0xFF);
    char fn[20];
    for (int i = 0; i < argc-1; i++) {
        strcpy(fn, "out/apps/");
        insert_file(imagebuffer, strcat(fn, argv[i+1]), a);
    }

    // we also reset the starting directory of shell
    // to root / 0xFF because we can't do so in actual OS.
    // it should reside in segment 0x120 address MAX_INPUT
    // because current_dir located after input with len MAX_INPUT.
    imagebuffer[0x120][MAX_INPUT] = 0xFF;

    // Overwrite old file
    for (int i = 0; i < 2880; i++)
        fwrite(imagebuffer[i], 512, 1, image);

    fclose(image);
    printf("> Apps successfully injected to image!\n");
    return 0;
}
