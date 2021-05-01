global long_mode_start

section .text
bits 64

long_mode_start:
    xor ax,ax
    mov ss,ax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax

    mov dword [0xB8000], 0x2f4b2f4f    
    hlt
