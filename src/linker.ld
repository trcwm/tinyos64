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
        code_end = .;        
        *(.rodata*)
    }

    .data :
    {
        *(.data)
        . = ALIGN(4K);
    }

}
