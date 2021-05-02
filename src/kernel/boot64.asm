global long_mode_start
extern kernel_start

section .text
bits 64

long_mode_start:
    xor ax,ax
    mov ss,ax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax

    call kernel_start
    hlt
