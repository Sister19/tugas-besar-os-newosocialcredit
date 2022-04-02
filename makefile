all: diskimage bootloader kernel
tc:
	gcc tc_gen/tc_gen.c tc_gen/tc_lib -o tc_gen/tc_gen
	./tc_gen/tc_gen A
	bochs -f src/config/if2230.config || true
diskimage:
	dd if=/dev/zero of=out/system.img bs=512 count=2880
bootloader:
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc
stdlib:
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
shell:
	bcc -ansi -c -o out/shell/global.o src/c/shell/global.c
	bcc -ansi -c -o out/shell/shell.o src/c/shell/shell.c
	bcc -ansi -c -o out/shell/args.o src/c/shell/args.c
	bcc -ansi -c -o out/shell/mkdir.o src/c/shell/mkdir.c
	bcc -ansi -c -o out/shell/cat.o src/c/shell/cat.c
	bcc -ansi -c -o out/shell/cd.o src/c/shell/cd.c
	bcc -ansi -c -o out/shell/ls.o src/c/shell/ls.c
	bcc -ansi -c -o out/shell/mv.o src/c/shell/mv.c
	bcc -ansi -c -o out/shell/cp.o src/c/shell/cp.c
system:
	bcc -ansi -c -o out/system/screen.o src/c/screen.c
	bcc -ansi -c -o out/system/keyboard.o src/c/keyboard.c
	bcc -ansi -c -o out/system/filesystem.o src/c/filesystem.c
kernel: stdlib system shell
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o out/system/*.o out/shell/*.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1
run:
	bochs -f src/config/if2230.config || true
build-run: all run
tc-run: build-run tc