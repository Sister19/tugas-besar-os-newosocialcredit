all: clear diskimage bootloader kernel apps
clear:
	rm -r -f out
	mkdir out
	mkdir out/apps
	mkdir out/system
tc:
	gcc tc_gen/tc_gen.c tc_gen/tc_lib -o tc_gen/tc_gen
	./tc_gen/tc_gen D
	bochs -f src/config/if2230.config || true
diskimage:
	dd if=/dev/zero of=out/system.img bs=512 count=2880
bootloader:
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc
stdlib:
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
apps: stdlib
	bcc -ansi -c -o out/apps/mkdir.o src/c/apps/mkdir.c
	bcc -ansi -c -o out/apps/cat.o src/c/apps/cat.c
	bcc -ansi -c -o out/apps/cd.o src/c/apps/cd.c
	bcc -ansi -c -o out/apps/ls.o src/c/apps/ls.c
	bcc -ansi -c -o out/apps/mv.o src/c/apps/mv.c
	bcc -ansi -c -o out/apps/cp.o src/c/apps/cp.c
	bcc -ansi -c -o out/apps/args.o src/c/apps/args.c
	bcc -ansi -c -o out/shell.o src/c/apps/shell.c
	bcc -ansi -c -o out/syscall.o src/c/syscall.c
	ld86 -o out/apps/utility -d out/shell.o out/apps/*.o out/interrupt.o out/syscall.o out/std_lib.o

	# #bcc -ansi -S -o out/apps/test.s src/c/apps/test.c
	# bcc -ansi -c -o out/apps/test.o src/c/apps/test.c
	# bcc -ansi -c -o out/syscall.o src/c/syscall.c
	# ld86 -o out/apps/utility -d out/apps/*.o out/interrupt.o out/syscall.o out/std_lib.o
	dd if=out/apps/utility of=out/system.img bs=512 conv=notrunc seek=272
system:
	bcc -ansi -c -o out/system/screen.o src/c/system/screen.c
	bcc -ansi -c -o out/system/keyboard.o src/c/system/keyboard.c
	bcc -ansi -c -o out/system/filesystem.o src/c/system/filesystem.c
kernel: stdlib system
	bcc -ansi -c -o out/global.o src/c/global.c
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/interrupt.asm -o out/interrupt.o
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/global.o out/interrupt.o out/system/*.o out/std_lib.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1
run:
	bochs -f src/config/if2230.config || true
build-run: all run
tc-run: build-run tc