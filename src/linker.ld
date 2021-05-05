ENTRY(boot32)

SECTIONS
{
    . = 1M;

    .boot :
    {        
        KEEP(*(.header));
        . = ALIGN(4K);
    }
    
    .text :
    {
        code_start = .;
        *(.text)
        *(.rodata*)
        . = ALIGN(4K);
        code_end = .;
    }

    .data :
    {
        *(.data)
        . = ALIGN(4K);
    }

}
