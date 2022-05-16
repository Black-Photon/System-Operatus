SRC := ./src
BUILD := ./build

cdimage.iso: fat.img
	mkdir -p $(BUILD)/iso
	cp $(BUILD)/fat.img $(BUILD)/iso
	xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o $(BUILD)/cdimage.iso $(BUILD)/iso

fat.img: BOOTX64.EFI
	dd if=/dev/zero of=$(BUILD)/fat.img bs=1k count=1440
	mformat -i $(BUILD)/fat.img -f 1440 ::
	mmd -i $(BUILD)/fat.img ::/EFI
	mmd -i $(BUILD)/fat.img ::/EFI/BOOT
	mcopy -i $(BUILD)/fat.img $(BUILD)/BOOTX64.EFI ::/EFI/BOOT

BOOTX64.EFI:
	mkdir -p $(BUILD)
	cd $(SRC) && make
	mv $(SRC)/target.efi $(BUILD)/BOOTX64.EFI
	cd $(SRC) && make clean

clean:
	rm -r $(BUILD)/*