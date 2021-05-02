# TinyOS64 - a very small bare-bones operating system for 64-bit x86

# Build requirements (Linux)
* NASM
* GCC x86_64 compiler
* Ninja build
* CMake
* grub-pc-bin
* grub-common
* xorriso
* qemu

run 'bootstrap.sh', go into 'build', run 'ninja dist'. There should be an .iso file in 'dist'.
run 'run_qemu.sh' to boot a VM and see the results.
