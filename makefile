OUT_DIR := out
SYSTEM_IMG := system.img
INTERRUPT_SRC := interrupt.asm
KERNEL_NAME := kernel
SRC_DIR := src/c
SRC_ASM_DIR := src/asm
LIBRARY_DIR := /library
SRC_LIB := $(SRC_DIR)$(LIBRARY_DIR)
OUT_LIB := $(OUT_DIR)$(LIBRARY_DIR)
LIST_LIBRARIES := $(notdir $(wildcard $(SRC_LIB)/*.c))
LIST_LIB_NO_SYSCALL := $(filter-out syscall.c,$(LIST_LIBRARIES))
APPS_DIR := /apps
SRC_APPS := $(SRC_DIR)$(APPS_DIR)
OUT_APPS := $(OUT_DIR)$(APPS_DIR)
LIST_APPS := $(notdir $(wildcard $(SRC_APPS)/*.c))
SYSTEM_DIR := /system
SRC_SYSTEM := $(SRC_DIR)$(SYSTEM_DIR)
OUT_SYSTEM := $(OUT_DIR)$(SYSTEM_DIR)
LIST_SYSTEMS := $(notdir $(wildcard $(SRC_SYSTEM)/*.c))
KERNEL_DIR := /kernel
SRC_KERNEL := $(SRC_DIR)$(KERNEL_DIR)
OUT_KERNEL := $(OUT_DIR)$(KERNEL_DIR)
CC := @bcc -ansi -c -o
CC_ASM := @nasm -f as86
LINKER := @ld86 -o

define \n


endef
	
all: clean diskimage bootloader kernel # apps
clean:
	@echo "> Clearing older build"
	@rm -r -f $(OUT_DIR)
	@echo "> Creating build folder"
	@mkdir $(OUT_DIR)
	@mkdir $(OUT_APPS)
	@mkdir $(OUT_LIB)
	@mkdir $(OUT_SYSTEM)
	@mkdir $(OUT_KERNEL)
tc:
	gcc tc_gen/tc_gen.c tc_gen/tc_lib -o tc_gen/tc_gen
	./tc_gen/tc_gen A
diskimage:
	@echo "> Building disk image"
	@dd if=/dev/zero of=$(OUT_DIR)/$(SYSTEM_IMG) bs=512 count=2880
bootloader:
	@echo "> Compiling bootloader"
	@nasm $(SRC_ASM_DIR)/bootloader.asm -o $(OUT_DIR)/bootloader
	@echo "> Copying bootloader to disk image"
	@dd if=$(OUT_DIR)/bootloader of=$(OUT_DIR)/$(SYSTEM_IMG) bs=512 count=1 conv=notrunc
library:
	@echo "> Compiling libraries"
	$(foreach file,$(LIST_LIBRARIES),\
	$(CC) $(OUT_LIB)/$(file:%.c=%.o) $(SRC_LIB)/$(file)${\n})
apps:
	@echo "> Compiling applications"
	$(foreach file,$(LIST_APPS),\
	$(CC) $(OUT_APPS)/$(file:%.c=%.o) $(SRC_APPS)/$(file)${\n}\
	$(LINKER) $(OUT_APPS)/$(basename $(file)) -d $(OUT_APPS)/$(file:%.c=%.o) \
		$(OUT_LIB)/interrupt.o \
		$(foreach lib,$(LIST_LIBRARIES),$(OUT_LIB)/$(lib:%.c=%.o) )${\n})
	# bcc -ansi -c -o out/apps/mkdir.o src/c/apps/mkdir.c
	# bcc -ansi -c -o out/apps/cat.o src/c/apps/cat.c
	# bcc -ansi -c -o out/apps/cd.o src/c/apps/cd.c
	# bcc -ansi -c -o out/apps/ls.o src/c/apps/ls.c
	# bcc -ansi -c -o out/apps/mv.o src/c/apps/mv.c
	# bcc -ansi -c -o out/apps/cp.o src/c/apps/cp.c
	# bcc -ansi -c -o out/apps/args.o src/c/apps/args.c
	# bcc -ansi -c -o out/shell.o src/c/apps/shell.c
	# bcc -ansi -c -o out/syscall.o src/c/syscall.c
	# ld86 -o out/apps/utility -d out/shell.o out/interrupt.o out/syscall.o out/std_lib.o

	# #bcc -ansi -S -o out/apps/test.s src/c/apps/test.c
	# bcc -ansi -c -o out/shell.o src/c/apps/test.c
	# bcc -ansi -c -o out/syscall.o src/c/syscall.c
	# bcc -ansi -c -o out/shell_common.o src/c/apps/shell_common.c
	# ld86 -o out/shell -d out/shell_common.o out/shell.o out/interrupt.o out/syscall.o out/std_lib.o
	# dd if=out/shell of=out/system.img bs=512 conv=notrunc seek=272
system:
	@echo "> Compiling system"
	$(foreach file,$(LIST_SYSTEMS),\
	$(CC) $(OUT_SYSTEM)/$(file:%.c=%.o) $(SRC_SYSTEM)/$(file)${\n})
kernel: library system
	@echo "> Compiling kernel"
	$(CC) $(OUT_KERNEL)/$(KERNEL_NAME).o $(SRC_KERNEL)/$(KERNEL_NAME).c
	$(CC_ASM) $(SRC_ASM_DIR)/$(INTERRUPT_SRC) -o $(OUT_LIB)/$(INTERRUPT_SRC:%.asm=%.o)
	$(CC_ASM) $(SRC_ASM_DIR)/$(KERNEL_NAME).asm -o $(OUT_KERNEL)/$(KERNEL_NAME)_asm.o
	$(LINKER) $(OUT_KERNEL)/$(KERNEL_NAME) -d $(OUT_KERNEL)/$(KERNEL_NAME).o \
		$(OUT_KERNEL)/$(KERNEL_NAME)_asm.o \
		$(OUT_LIB)/$(INTERRUPT_SRC:%.asm=%.o) \
		$(foreach file,$(LIST_SYSTEMS),$(OUT_SYSTEM)/$(file:%.c=%.o) ) \
		$(foreach lib,$(LIST_LIB_NO_SYSCALL),$(OUT_LIB)/$(lib:%.c=%.o) )
	@echo "> Copying kernel to disk image"
	@dd if=$(OUT_DIR)$(KERNEL_DIR)/$(KERNEL_NAME) of=$(OUT_DIR)/$(SYSTEM_IMG) bs=512 conv=notrunc seek=1
run:
	bochs -f src/config/if2230.config || true
build-run: all run
tc-run: build-run tc
	bochs -f src/config/if2230.config || true