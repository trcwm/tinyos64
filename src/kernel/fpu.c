#include "fpu.h"

void fpu_load_control_word(const uint16_t control)
{
    asm volatile("fldcw %0;"::"m"(control)); 
}

void fpu_init()
{
    asm volatile("fninit");
}
