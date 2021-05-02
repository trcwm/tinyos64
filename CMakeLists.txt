
cmake_minimum_required(VERSION 3.12)
project(tinyos64)

add_subdirectory(src)

add_custom_target(dist DEPENDS kernel
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/dist/tinyos64.iso ${CMAKE_SOURCE_DIR}/iso/boot/kernel.bin
    COMMAND cp ${CMAKE_SOURCE_DIR}/dist/kernel.bin ${CMAKE_SOURCE_DIR}/iso/boot/kernel.bin
    COMMAND grub-mkrescue /usr/lib/grub/i386-pc -o ${CMAKE_SOURCE_DIR}/dist/tinyos64.iso ${CMAKE_SOURCE_DIR}/iso)
