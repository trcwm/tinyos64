project(kernel)

add_custom_command(OUTPUT boot.o multiboot.o
    COMMAND nasm -f elf64 ${PROJECT_SOURCE_DIR}/boot.asm -o boot.o
    COMMAND nasm -f elf64 ${PROJECT_SOURCE_DIR}/multiboot.asm -o multiboot.o)

add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/dist/kernel.bin
    DEPENDS boot.o
    COMMAND ld -n --oformat elf64-little -o ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin -T ${PROJECT_SOURCE_DIR}/linker.ld  boot.o multiboot.o
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin ${CMAKE_SOURCE_DIR}/dist/kernel.bin
    )

add_custom_target(kernel 
    DEPENDS ${CMAKE_SOURCE_DIR}/dist/kernel.bin
    )
