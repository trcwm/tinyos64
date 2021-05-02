project(kernel)

enable_language(C ASM_NASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

add_executable(kernel 
    kernel/multiboot.c
    kernel/boot.asm
    kernel/boot64.asm
    kernel/main.c
    )

#target_compile_options(kernel PUBLIC -static -nodefaultlibs -nostartfiles)
target_link_options(kernel PUBLIC -static -nodefaultlibs -nostartfiles -T ${CMAKE_SOURCE_DIR}/src/linker.ld)

add_custom_command(TARGET kernel 
    POST_BUILD
    COMMAND cp kernel ${CMAKE_SOURCE_DIR}/dist/kernel.bin
    COMMAND strip ${CMAKE_SOURCE_DIR}/dist/kernel.bin
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/dist/kernel.bin
)
