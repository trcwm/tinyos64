#!/bin/sh
qemu-system-x86_64 -cdrom dist/tinyos64_efi.iso -monitor stdio -d int -m 4096 -cpu max
