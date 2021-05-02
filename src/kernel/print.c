/*

    Tiny printf for embedded systems

    Niels A. Moseley
    Copyright (c) 2017-2021 Moseley Instruments
    All rights reserved

*/

#include <stdarg.h>
#include <stdbool.h>
#include "print.h"
#include "console.h"

/** convert a 32-bit number to an 8-digit ASCII hex number */
static void toHex(uint32_t val)
{
    for(uint32_t i=0; i<8; i++)
    {
        uint32_t digit = (val >> ((7-i)*4)) & 0x0F;
        if (digit <= 9)
            console_outch('0'+digit);
        else
            console_outch('A'+digit-10);
    }
}

/** convert a 32-bit number to a 10-digit ASCII number */
static size_t toUDec(uint32_t val)
{
    size_t chars = 0;
    uint8_t digits[10] = {0,0,0,0,0,0,0,0,0,0};
    
    for(uint32_t s=0; s<32; s++)
    {
        uint8_t overflow = (val & 0x80000000) ? 1 : 0;
        val <<= 1;
        for(uint32_t d=0; d<10; d++)
        {
            digits[d] = (digits[d] << 1) | overflow;
            if (digits[d] >= 10)
            {
                overflow = 1;
                digits[d] -= 10;
            }
            else
            {
                overflow = 0;
            }
        }
    }
    
    // strip leading zeros
    bool leadingZero = true;
    for(uint32_t i=0; i<10; i++)
    {
        if ((digits[9-i] == 0) && (leadingZero))
        {
            // do not print leading zeros
        }
        else
        {
            chars++;
            console_outch(digits[9-i] + '0');
            leadingZero = false;
        }
    }

    // check if we've had only leading zeros and
    // thus haven't printed anything.. :)
    if (leadingZero)
    {
        chars++;
        console_outch('0');
    }

    return chars;
}


static void format(const char *fmt, va_list va)
{
    size_t tmp;
    uint32_t vtmp;
    char *charptr;
    char ch;

    while((ch=*(fmt++)) !=0)
    {
        if (ch != '%')
        {
            console_outch(ch);
        }
        else
        {
            ch=*fmt++;
            switch(ch)
            {
            case 'X':
                toHex(va_arg(va, uint32_t));
                break;
            case 'u':
                tmp = toUDec(va_arg(va, uint32_t));
                break;
            case 'd':
                vtmp =  va_arg(va, uint32_t);
                if ((vtmp & 0x80000000) != 0)
                {
                    // sign bit, we have a negative number
                    vtmp = ~vtmp;   // invert bits
                    vtmp++;         // add one

                    console_outch('-');
                    tmp = toUDec(vtmp);
                }
                else
                {
                    tmp = toUDec(vtmp);
                }
                break;
            case 's':
                charptr = va_arg(va, char*);
                while(*charptr != 0)
                {
                    console_outch(*charptr++);
                }
                break;
            case 'c':
                vtmp = va_arg(va, int);
                console_outch(vtmp);
                break;                
            case '%':
                console_outch('%');
                break;
            default:
                console_outch('?');
                break;
            }
        }
    }
}

size_t print(const char *fmt, ...)
{
    va_list va;
    va_start(va,fmt);

    format(fmt,va);
    va_end(va);

    return 0;
}
