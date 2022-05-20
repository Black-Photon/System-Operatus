qemu-system-x86_64 -cpu qemu64 -machine q35 `
  -drive if=pflash,format=raw,unit=0,file="libs/ovmf-blobs/OVMF_CODE-pure-efi.fd",readonly=on `
  -drive if=pflash,format=raw,unit=1,file="libs/ovmf-blobs/OVMF_VARS-pure-efi.fd" `
  -drive file=build/cdimage.iso,if=ide `
  -drive id=disk,file=drive.img,if=none `
  -device ahci,id=ahci `
  -device ide-hd,drive=disk,bus=ahci.0 `
  -net none