#include "console.h"
#include "keyboard.h"
#include "print.h"

void __stack_chk_fail()
{
    while(1) {};
}

void kernel_start()
{
    console_clear();
    print("Hello from 64-bit C!\n\r\n\r");
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
