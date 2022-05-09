BUILD_DIR := ./build
SRC_DIR := ./src
ISO_DIR := $(BUILD_DIR)/isodir
BOOT_DIR := $(ISO_DIR)/boot
GRUB_DIR := $(BOOT_DIR)/grub

GCC_DIR := ../Programs/i686-elf-tools-windows

CC=$(GCC_DIR)/bin/i686-elf-gcc.exe
AS=$(GCC_DIR)/bin/i686-elf-as.exe

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

jOSeph.iso: jOSeph.elf
	mkdir -p $(GRUB_DIR)
	cp $(BUILD_DIR)/jOSeph.elf $(BOOT_DIR)/jOSeph.elf
	cp $(SRC_DIR)/grub.cfg $(GRUB_DIR)/grub.cfg
	wsl grub-mkrescue -o $(BUILD_DIR)/jOSeph.iso $(ISO_DIR)

jOSeph.elf: boot.o kernel.o
	$(CC) -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/jOSeph.elf -ffreestanding -O2 -nostdlib $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o -lgcc

kernel.o:
	mkdir -p build
	$(CC) -c $(SRC_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o $(CFLAGS) 

boot.o:
	mkdir -p build
	$(AS) $(SRC_DIR)/boot.s -o $(BUILD_DIR)/boot.o

clean:
	rm -r $(BUILD_DIR)