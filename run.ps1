qemu-system-x86_64 -cpu qemu64 -machine q35 `
  -drive if=pflash,format=raw,unit=0,file="libs/ovmf-blobs/OVMF_CODE-pure-efi.fd",readonly=on `
  -drive if=pflash,format=raw,unit=1,file="libs/ovmf-blobs/OVMF_VARS-pure-efi.fd" `
  -drive file=build/cdimage.iso,if=ide `
  -drive file=drive.img,format=raw,media=disk `
  -net none