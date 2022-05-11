CC := x86_64-w64-mingw32-gcc

GNU_EFI := ./libs/gnu-efi-3.0.14
OVMF := ./libs/ovmf-blobs/bios64.bin

SRC := ./src
BUILD := ./build

cdimage.iso: fat.img
	mkdir $(BUILD)/iso
	cp $(BUILD)/fat.img $(BUILD)/iso
	xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o $(BUILD)/cdimage.iso $(BUILD)/iso

fat.img: BOOTX64.EFI
	dd if=/dev/zero of=$(BUILD)/fat.img bs=1k count=1440
	mformat -i $(BUILD)/fat.img -f 1440 ::
	mmd -i $(BUILD)/fat.img ::/EFI
	mmd -i $(BUILD)/fat.img ::/EFI/BOOT
	mcopy -i $(BUILD)/fat.img $(BUILD)/BOOTX64.EFI ::/EFI/BOOT

BOOTX64.EFI: hello.o data.o
	$(CC) -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o $(BUILD)/BOOTX64.EFI $(BUILD)/hello.o $(BUILD)/data.o

hello.o:
	mkdir -p $(BUILD)
	$(CC) -ffreestanding -I"$(GNU_EFI)/inc" -I"$(GNU_EFI)/inc/x86_64" -I"$(GNU_EFI)/inc/protocol" -c -o $(BUILD)/hello.o $(SRC)/hello.c

data.o:
	mkdir -p $(BUILD)
	$(CC) -ffreestanding -I"$(GNU_EFI)/inc" -I"$(GNU_EFI)/inc/x86_64" -I"$(GNU_EFI)/inc/protocol" -c -o $(BUILD)/data.o $(SRC)/gnu-efi/lib/data.c

clean:
	rm -r build