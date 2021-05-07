// https://github.com/Stichting-MINIX-Research-Foundation/minix/tree/master/minix/kernel/arch/i386

#include "console.h"
#include "keyboard.h"
#include "print.h"
#include "fpu.h"
#include "ports.h"
#include "smbios.h"
#include "mandelbrot.h"

extern uint64_t _idtr;
extern uint64_t _isr_table;
extern uint64_t stack_top;
extern uint64_t stack_bottom;
extern uint64_t code_start;
extern uint64_t code_end;

void __stack_chk_fail()
{
    while(1) {};
}

#include "exceptions.c"

void dump_isrtable()
{
    uint64_t *tbl = &_isr_table;
    for(uint32_t i=0; i<16; i++)
    {
        uint64_t lower  = tbl[0] & 0x0FFFF;
        uint64_t middle = tbl[0] >> (12*4);
        uint64_t high   = tbl[1] & 0x0FFFFFFFF;
        uint64_t addr   = lower | (middle << 16) | (high << 32);
        print("ISR %d:  %lX  %lX  addr=%lX\n\r", i, tbl[0], tbl[1], addr);
        tbl+=2;
    }
}

bool strncmp(const char *s1, const char *s2, size_t n)
{
    for(size_t i=0; i<n; i++)
    {
        if (*s1 != *s2)
            return false;

        if ((*s1 == *s2) && (*s1 == 0))
        {
            // early string termination
            return true;
        }

        ++s1;
        ++s2;
    }

    return true;
}

void kernel_start()
{
    fpu_init();

    console_clear();
    print("Hello from 64-bit C!\n\r\n\r");
    
    print("CS   : 0x%lX\n\r", read_cs());
    print("IDTR : 0x%lX\n\r", _idtr);
    print("CR0  : 0x%lX\n\r", read_cr0());
    print("CR4  : 0x%lX\n\r", read_cr4());
    print("EFER : 0x%lX\n\r", read_msr(0xC0000080));  // EFER MSR 
    
    print("Stack top     : 0x%lX\n\r", &stack_top);
    print("Stack bottom  : 0x%lX\n\r", &stack_bottom);
    print("Stack size is : 0x%lX bytes\n\r\n\r", &stack_top - &stack_bottom);

    print("Code start    : 0x%lX\n\r", &code_start);
    print("Code end      : 0x%lX\n\r\n\r", &code_end);

    uint64_t* isrptr = (uint64_t*)(((uint8_t*)&_idtr)+2);
    print("_isr_table is at : 0x%lX\n\r", &_isr_table);
    print("_isr_table check : 0x%lX\n\r", *isrptr);
    print("_isr_table is    : %d bytes\n\r", (_idtr & 0xFFFF) + 1);

    uint64_t sptr = read_stackptr();
    print("Stack ptr is at address %lX\n\r", sptr);

    setISR(0, (uint64_t)handler0);
    setISR(1, (uint64_t)handler1);
    setISR(2, (uint64_t)handler2);
    setISR(3, (uint64_t)handler3);
    setISR(4, (uint64_t)handler4);
    setISR(5, (uint64_t)handler5);
    setISR(6, (uint64_t)handler6);
    setISR(7, (uint64_t)handler7);
    setISR(8, (uint64_t)handler8);
    setISR(9, (uint64_t)handler9);
    setISR(10, (uint64_t)handler10);
    setISR(11, (uint64_t)handler11);
    setISR(12, (uint64_t)handler12);
    setISR(13, (uint64_t)handler13);
    setISR(14, (uint64_t)handler14);
    setISR(15, (uint64_t)handler15);

    for(uint8_t i=16; i<50; i++)
    {
        setISR(i, (uint64_t)defaulthandler);
    }

    //enableInterrupts();

    print("\n\r");

    // check for SSE instruction support
    uint64_t v = cpuid(0x1);
    if ((v & (1<<25)) != 0)
    {
        print("SSE supported\n\r");
    }
    else
    {
        print("No SSE\n\r");
    }    
    if ((v & (1<<26)) != 0)
    {
        print("SSE2 supported\n\r");
    }
    else
    {
        print("No SSE2\n\r");
    }

#if 0
    // detect smbios
    void* smbios = detect_smbios();

    if (smbios != 0)
    {
        print("SMBios at 0x%lX\n\r", smbios);
        SMBios32Header_t* smbios32 = (SMBios32Header_t*)smbios;
        SMBios64Header_t* smbios64 = (SMBios64Header_t*)smbios;

        uint8_t* ptr = 0;   // pointer to SMBios structures

        if (strncmp(smbios32->magic, "_SM_", 4))
        {
            // 32 bit version
            print("  32 bit version\n\r");
            ptr = (uint8_t*)(uint64_t)smbios32->struct_tbl_address;
            print("  number of structs: %d\n\r", smbios32->num_structs);
        }
        else if (strncmp(smbios64->magic, "_SM3_", 5))
        {
            // 64 bit version
            print("  64 bit version\n\r");
            ptr = (uint8_t*)smbios64->struct_tbl_address;
        } 

        //FIXME: this only works for 32 bit SMBIOS!!
        size_t count = smbios32->num_structs;
        size_t scount = 0;
        while (count != 0)
        {
            SMStructHeader_t* smbs = (SMStructHeader_t*)ptr;
            uint8_t stype = smbs->stype;            
            //print("  struct %d\n\r", scount++);
            print("  type   = %d\n\r", smbs->stype);
            //print("  len    = %d bytes\n\r", smbs->len);
            //print("  handle = 0x%X\n\r", smbs->handle);

            // skip formatted area
            ptr += smbs->len;

            if (stype == 0 /* bios info */)
            {
                SMBiosInfoFormatted_t *info = (SMBiosInfoFormatted_t *)ptr;

                // print the characteristics
                // see: https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.4.0.pdf
                // page 35
                // 

                if (info->biosCharacteristics & (1<<30))
                {
                    print("    int 10h CGA/Mono video services supported\n\r");
                }
                if (info->biosCharacteristics & (1<<28))
                {
                    print("    int 4h serial services supported\n\r");
                }                
                if (info->biosCharacteristics & (1<<27))
                {
                    print("    int 9h 8042 keyboard services supported\n\r");
                }
                if (info->biosCharacteristics & (1<<26))
                {
                    print("    int 5h screen printing services supported\n\r");
                }
                if (info->biosCharacteristicsExt[0] & 1)
                {
                    print("    ACPI supported\n\r");
                }
                if (info->biosCharacteristicsExt[0] & (1<<1))
                {
                    print("    legacy USB supported\n\r");
                }
                if (info->biosCharacteristicsExt[1] & (1<<3))
                {
                    print("    UEFI supported\n\r");
                }
                if (info->biosCharacteristicsExt[1] & (1<<4))
                {
                    print("    SMBios = virtual machine\n\r");
                }
            }

            // show strings
            while(true)
            {
                //print("\t");
                while(*ptr != 0)
                {
                    ptr++;
                //    print("%c", *ptr++);
                }
                //print("\n\r");
                
                ptr++;
                if (*ptr == 0)
                {
                    ptr++;
                    break;                        
                }
            }
            count--;
        }
    }
    else
    {
        print("SMBios not found!\n\r");
    }
#endif 

    //dump_isrtable();
#if 0
    const char gfx[] = " .,'~!^:;[/<&?oxOX#  ";
    for(int32_t y=-12; y<12; y++)
    {
        float im = y/6.0f;

        for(int32_t x=-39; x<39; x++)
        {
            float re = -0.25f + x/15.0f;
            volatile uint32_t iter = mandel_iter(re, im, 20);
            print("%c", gfx[iter]);
        }
        print("\n\r");
    }
#endif
    print("\n\r");
    print(">");
    {
        uint8_t kb = keyboard_read();
        if (kb == '\n')
        {
            print("\n\r>", kb);
        }
        else
        {
            print("%c", kb);
        }
    }
}
