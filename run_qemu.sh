#!/bin/sh
qemu-system-x86_64 -cdrom dist/tinyos64.iso -monitor stdio -d int -m 4096 -cpu max

#-cpu SandyBridge,+avx,+pdpe1gb,+sse,+sse2