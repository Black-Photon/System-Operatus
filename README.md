# jOSeph v5

## Overview
Attempt to create a new Operating System based on tutorials from https://wiki.osdev.org

## Dependancies
- Must have the GCC cross-compiler for i686 installed into `./libs/i686-elf-tools-windows` (may wish to use provided at https://github.com/lordmilko/i686-elf-tools/releases/tag/7.1.0)
- Requires wsl installed with the following packages:
  - `xorriso`
  - `qemu`
  - `ovmf`
  - `gnu-efi`
  - `binutils-mingw-w64`
  - `gcc-mingw-w64`
  - `mtools`
  - `mkgpt` (can install from https://github.com/jncronin/mkgpt.git)
- Install the Qemu VM
- Requires OVMF binaries

## Build instructions
Instructions assume using Windows
1) Build using `make`
2) Run using `./run.ps1`