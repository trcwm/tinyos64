global long_mode_start
extern kernel_start
extern stack_top
extern _idtr

section .text
bits 64

long_mode_start:
    xor ax,ax
    mov ss,ax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax

    mov esp, stack_top

    call setup_fpu

    lidt [_idtr]
    call kernel_start
    hlt

    call setup_fpu

setup_fpu:
    mov rdx, cr0
    and rdx, 0xFFFFFFFFFFFFFFFF - (1<<2) - (1<<5)   ; reset NE and EM bits
    or  rdx, 0x2			                ; set coprocessor monitoring CR0.MP
    mov cr0, rdx
    
    mov rdx, cr4
    or  rdx, 3 << 9		;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
    or  rdx, 1 << 18		;Enables the XSAVE extension
    mov cr4, rdx

    fninit
    ret
