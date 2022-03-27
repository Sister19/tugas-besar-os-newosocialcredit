all: diskimage bootloader kernel
diskimage:
	dd if=/dev/zero of=out/system.img bs=512 count=2880
bootloader:
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc
stdlib:
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
	bcc -ansi -c -o out/screen.o src/c/screen.c
	bcc -ansi -c -o out/keyboard.o src/c/keyboard.c
	bcc -ansi -c -o out/filesystem.o src/c/filesystem.c
	bcc -ansi -c -o out/shell.o src/c/shell.c
kernel: stdlib
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o out/screen.o out/keyboard.o out/filesystem.o out/shell.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1
run:
	bochs -f src/config/if2230.config
build-run: all run