#include "ports.h"

void halt()
{
    asm volatile ( "hlt" );
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

uint64_t read_stackptr()
{
    uint64_t val;
    asm volatile ( "mov %%rsp, %0" : "=r"(val) );
    return val;    
}

uint64_t read_cr0()
{
    //https://wiki.osdev.org/Inline_Assembly/Examples#READ_CRx
    uint64_t val;
    asm volatile ( "mov %%cr0, %0" : "=r"(val) );
    return val;    
}

uint64_t read_cr4()
{
    uint64_t val;
    asm volatile ( "mov %%cr4, %0" : "=r"(val) );
    return val;    
}

uint64_t read_cs()
{
    uint64_t val;
    asm volatile ( "mov %%cs, %0" : "=r"(val) );
    return val;    
}

uint64_t cpuid(uint64_t index)
{
    uint64_t val;
    asm volatile("cpuid" : "=d"(val) : "a"(index) );
    return val;    
}

uint64_t read_msr(const uint32_t msr)
{
    uint64_t result;
    uint32_t *lo = (uint32_t*)&result;
    uint32_t *hi = lo+1;
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
    return result;
}
 
void write_msr(const uint32_t msr, const uint64_t val)
{
    uint32_t lo = val;
    uint32_t hi = val >> 32;
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

extern uint8_t* _isr_table;

void setISR(uint8_t intnum, uint64_t address)
{
    // an ISR table entry is 16 bytes in long mode

    uint16_t* ptr = (uint16_t*)&_isr_table;
    ptr += intnum*8;

    ptr[0] = address & 0x0FFFF;  // low offset
    ptr[1] = 8;              // code selector
    ptr[2] = 0x8e00;         // type and attributes
    ptr[3] = (address >> 16) & 0x0FFFF;
    ptr[4] = (address >> 32) & 0x0FFFF;
    ptr[5] = (address >> 48) & 0x0FFFF;
    ptr[6] = 0;
    ptr[7] = 0;
}

void int10()
{
    //asm volatile ("int qword 10");   
}
