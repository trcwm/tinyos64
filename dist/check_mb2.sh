#!/bin/sh
# check if the kernel.bin is multiboot2 compatible

if ! grub-file --is-x86-multiboot2 kernel.bin ; then
    echo "ERROR!"
else
    echo "OK!"
fi
