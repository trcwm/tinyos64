// https://wiki.osdev.org/Inline_Assembly

#include <stdint.h>
#include "ports.h"

typedef struct 
{
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;    
    uint32_t tabSize;
} ConsoleState;

typedef struct
{
    uint8_t character;
    uint8_t colour;
} ConsoleChar;

static ConsoleState consoleState =
{
    .x = 0,
    .y = 0,
    .h = 25,
    .w = 80,
    .tabSize = 8
};

void console_clear()
{
    ConsoleChar *screenPtr = (ConsoleChar *)(0xB8000);
    consoleState.x = 0;
    consoleState.y = 0;
    
    for(uint32_t y=0; y<consoleState.h; y++)
    {
        for(uint32_t x=0; x<consoleState.w; x++)
        {
            screenPtr->character = 32;
            screenPtr->colour    = 2;
            screenPtr++;
        }
    }
}

void console_set_cursor(const uint32_t x, const uint32_t y)
{
	uint16_t pos = y * consoleState.w + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void console_xy(const uint32_t x, const uint32_t y)
{
    consoleState.x = x;
    consoleState.y = y;
    
    if (consoleState.x >= consoleState.w)
    {
        consoleState.x = consoleState.w-1;
    }
    
    if (consoleState.y >= consoleState.h)
    {
        consoleState.y = consoleState.h-1;
    }

    console_set_cursor(consoleState.x, consoleState.y);
}

void console_scroll()
{
    ConsoleChar *dst = (ConsoleChar *)(0xB8000);
    ConsoleChar *src = dst + consoleState.w;
    
    for(uint32_t y=0; y<(consoleState.h-1); y++)
    {
        for(uint32_t x=0; x<consoleState.w; x++)
        {
            *dst++ = *src++;
        }   
    }
    
    // clear the last line
    for(uint32_t x=0; x<consoleState.w; x++)
    {
        dst->character = 32;
        dst->colour    = 2;
        dst++;
    }
}

void console_adjust()
{
    if (consoleState.x >= consoleState.w)
    {
        consoleState.x = 0;
        consoleState.y++;
    }
    
    if (consoleState.y >= consoleState.h)
    {
        // we need to scroll
        console_scroll();
        consoleState.x = 0;
        consoleState.y = consoleState.h - 1;
    }

    console_set_cursor(consoleState.x, consoleState.y);
}

void console_outch(const char c)
{
    if (c == 10)    // linefeed
    {
        consoleState.y++;
        console_adjust();
    }
    else if (c == 13)   // carriage return
    {
        consoleState.x = 0;
        console_set_cursor(consoleState.x, consoleState.y);
    }
    else if (c == '\t') // tab
    {
        consoleState.x += consoleState.tabSize;
        consoleState.x = (consoleState.x / consoleState.tabSize) * consoleState.tabSize;
        console_adjust();
    }
    else if (c >= 32)
    {
        ConsoleChar *screenPtr = (ConsoleChar *)(0xB8000UL + consoleState.x*2 + consoleState.y*consoleState.w*2);
        screenPtr->character = c;
        consoleState.x++;
        console_adjust();
    }
}

void console_write(const char *str)
{
    while(*str != 0)
    {
        char c = *str;
        console_outch(c);
        str++;
    }
}
