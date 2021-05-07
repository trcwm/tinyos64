; i386 boot code

global boot32
global stack_top
global stack_bottom
global _isr_table
global _idtr

extern long_mode_start

section .text
bits 32
boot32:
    cli
    mov esp, stack_top    

    call check_multiboot
    call check_cpuid
    call check_long_mode

    call setup_page_tables
    call enable_paging

    lgdt [gdt64.pointer]
    jmp gdt64.code_segment:long_mode_start
    
    hlt

check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al,"M"
    jmp error

check_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "C"
    jmp error

check_long_mode:
    mov eax,0x80000000
    cpuid
    cmp eax,0x80000001
    jb .no_long_mode

    mov eax,0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
.no_long_mode:
    mov al,"L"
    jmp error

setup_page_tables:
    mov eax, page_table_l3
    or  eax, 0b11   ;  present, writable
    mov [page_table_l4], eax

    mov eax, page_table_l2
    or  eax, 0b11   ;  present, writable
    mov [page_table_l3], eax

    xor ecx,ecx
.fill_loop:
    mov eax, 0x200000   ; 2MiB
    mul ecx
    or  eax, 0b10000011 ; present, writable, huge page
    mov [page_table_l2 + ecx*8], eax
    inc ecx
    cmp ecx, 512
    jne .fill_loop
    ret

enable_paging:    
    mov eax, page_table_l4
    mov cr3, eax

    ; disable write protect bit in CR0
    mov eax, cr0
    and eax, 0xFFFFFFFF - (1<<16) ; enable writing to read-only pages in ring 0
    mov cr0,eax 

%if 0
    xor edi, edi
    lea esi, [txt1]
    call print_string

    mov eax, cr0
    call print_eax

    add edi,4
    lea esi, [txt2]
    call print_string

    mov eax, cr4
    call print_eax
    hlt
%endif

    ; enable PAE
    ; enabling this makes my real system crash.. ??
    mov eax, cr4
    or  eax, 1 << 5
    mov cr4, eax

    ; enable long mode
    mov ecx, 0xC0000080
    rdmsr 
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or  eax, 1 << 31
    mov cr0, eax

    ret

error:
    mov byte [0xB8000],al
    hlt


; print eax to the screen 
; where edi is the offset into screen memory
print_eax:
    mov ecx, 32/4   ; number of nibbles
.ploop:
    rol eax,4       ; printable nibble in lsb
    mov edx,eax
    and edx, 0x0F    ; mask nibble
    movzx edx, byte [nibble2ascii + edx]
    mov byte [0xB8000 + edi*2],dl
    inc edi
    dec ecx
    jnz .ploop
    ret

print_string:
    mov al, byte [esi]
    cmp al,0
    je .pexit
    mov byte[0xB8000 + edi*2],al
    inc esi
    inc edi
    jmp print_string
.pexit:
    ret

section .bss

align 4096
page_table_l4:
    resb 4096
page_table_l3:
    resb 4096
page_table_l2:
    resb 4096
_isr_table:
    resb 50*16      ; reserve 50 entries in interrupt table

stack_bottom:
    resq 16384
stack_top:

section .rodata
gdt64:
    dq 0
.code_segment: equ $ - gdt64
    dq (1<<43) | (1 << 44) | (1 << 47) | (1 << 53)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

nibble2ascii: db "0123456789ABCDEF"
txt1: db "CR0: 0x",0
txt2: db "CR4: 0x",0

section .data
    ; IDT descriptor:
_idtr:
    dw   50*16-1    ; total bytes in isr_table
    dd   _isr_table ; pointer to the table
