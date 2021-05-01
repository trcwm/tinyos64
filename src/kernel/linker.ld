ENTRY(boot32)

SECTIONS
{
    . = 1M;

    .boot :
    {
        . = ALIGN(8);
        KEEP(*(.multiboot_header))
    }
    
    .text :
    {
        *(.text)
    }
}
