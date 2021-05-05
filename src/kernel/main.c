// https://github.com/Stichting-MINIX-Research-Foundation/minix/tree/master/minix/kernel/arch/i386

#include "console.h"
#include "keyboard.h"
#include "print.h"
#include "fpu.h"
#include "ports.h"
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
    //dump_isrtable();

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

    print("\n\r");
    print(">");
    while(1) 
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
