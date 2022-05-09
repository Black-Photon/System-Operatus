BUILD_DIR := ./build
SRC_DIRS := ./src

GCC_DIR=..\Programs\i686-elf-tools-windows

CC=$(GCC_DIR)\bin\i686-elf-gcc.exe
AS=$(GCC_DIR)\bin\i686-elf-as.exe

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

operatus.elf: boot.o kernel.o
	$(CC) -T $(SRC_DIRS)/linker.ld -o $(BUILD_DIR)/operatus.elf -ffreestanding -O2 -nostdlib $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o -lgcc

kernel.o:
	mkdir -p build
	$(CC) -c $(SRC_DIRS)/kernel.c -o $(BUILD_DIR)/kernel.o $(CFLAGS) 

boot.o:
	mkdir -p build
	$(AS) $(SRC_DIRS)/boot.s -o $(BUILD_DIR)/boot.o

clean:
	rm -r $(BUILD_DIR)