; i386 boot code

global boot32

section .text
bits 32
boot32:
    mov dword [0xB800], 0x2f4b2f4f
    hlt

