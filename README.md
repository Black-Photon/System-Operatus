# System Operatus

## Overview
Attempt to create a new Operating System based on tutorials from https://wiki.osdev.org

## Build instructions
Instructions assume using Windows
1) Firstly install the GCC cross-compiler for i686 into `..\Programs\i686-elf-tools-windows` (may wish to use provided at https://github.com/lordmilko/i686-elf-tools/releases/tag/7.1.0)
2) Then you can build the `.elf` by running `make operatus.elf`
3) Run the OS with Qemu VM using `qemu-system-i386 -kernel .\build\operatus.elf`