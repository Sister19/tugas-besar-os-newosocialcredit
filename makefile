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
LIST_LIB_SYSCALL := syscall.c fileio.c program.c shell_common.c textio.c args.c
LIST_LIB_NO_SYSCALL := $(filter-out $(LIST_LIB_SYSCALL),$(LIST_LIBRARIES))
APPS_DIR := /apps
SRC_APPS := $(SRC_DIR)$(APPS_DIR)
OUT_APPS := $(OUT_DIR)$(APPS_DIR)
LIST_ALL_APPS := $(notdir $(wildcard $(SRC_APPS)/*.c))
APPS ?= $(basename $(LIST_ALL_APPS))
LIST_APPS := $(foreach app,$(APPS),$(filter $(app).c,$(LIST_ALL_APPS)))
SYSTEM_DIR := /system
SRC_SYSTEM := $(SRC_DIR)$(SYSTEM_DIR)
OUT_SYSTEM := $(OUT_DIR)$(SYSTEM_DIR)
LIST_SYSTEMS := $(notdir $(wildcard $(SRC_SYSTEM)/*.c))
KERNEL_DIR := /kernel
SRC_KERNEL := $(SRC_DIR)$(KERNEL_DIR)
OUT_KERNEL := $(OUT_DIR)$(KERNEL_DIR)
CC := @bcc -ansi -c -O1 -o
CC_ASM := @nasm -f as86
LINKER := @ld86 -o
TC ?= A

define \n


endef

a:
	@echo $(LIST_APPS)
all: clean diskimage bootloader kernel apps inject
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
	gcc tc_gen/tc_lib.c -c -o tc_gen/tc_lib.o
	gcc tc_gen/tc_gen.c tc_gen/tc_lib.o -o tc_gen/tc_gen
	./tc_gen/tc_gen $(TC)
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
		$(OUT_LIB)/$(INTERRUPT_SRC:%.asm=%.o) \
		$(foreach lib,$(LIST_LIBRARIES),$(OUT_LIB)/$(lib:%.c=%.o) )${\n})
inject:
	@gcc tc_gen/tc_lib.c -c -o tc_gen/tc_lib.o
	@cp tc_gen/tc_lib.o src/c/utils
	@gcc src/c/utils/inject_apps.c src/c/utils/tc_lib.o -o src/c/utils/inject_apps
	./src/c/utils/inject_apps $(foreach file,$(LIST_APPS),$(basename $(file)))
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
tc-run: all tc run