project(kernel)

enable_language(C ASM_NASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

add_executable(kernel 
    kernel/multiboot.c
    kernel/boot.asm
    kernel/boot64.asm
    kernel/console.c
    kernel/ports.c
    kernel/keyboard.c
    kernel/print.c
    kernel/fpu.c
    kernel/main.c
    kernel/smbios.c
    kernel/mandelbrot.c
    )

target_link_options(kernel PUBLIC -static -z max-page-size=0x1000 -nodefaultlibs -nostartfiles -T ${CMAKE_SOURCE_DIR}/src/linker.ld)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpmath=387 -msse2 -fno-stack-protector -ffreestanding")


add_custom_command(TARGET kernel 
    POST_BUILD
    COMMAND cp kernel ${CMAKE_SOURCE_DIR}/dist/kernel.bin
    COMMAND objdump -d kernel >${CMAKE_SOURCE_DIR}/dist/kernel.dump
    COMMAND strip ${CMAKE_SOURCE_DIR}/dist/kernel.bin
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/dist/kernel.bin
)
