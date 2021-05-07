
#include <stdint.h>

__attribute__((naked)) void handler0()
{
    const char* txt = "INTERRUPT 0!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler1()
{
    const char* txt = "INTERRUPT 1!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler2()
{
    const char* txt = "INTERRUPT 2!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler3()
{
    const char* txt = "INTERRUPT 3!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler4()
{
    const char* txt = "INTERRUPT 4!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler5()
{
    const char* txt = "INTERRUPT 5!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler6()
{
    const char* txt = "INTERRUPT 6!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler7()
{
    const char* txt = "INTERRUPT 7!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler8()
{
    const char* txt = "INTERRUPT 8!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    asm("retq");

    while(1) {};
}

__attribute__((naked)) void handler9()
{
    const char* txt = "INTERRUPT 9!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler10()
{
    const char* txt = "INTERRUPT 10!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler11()
{
    const char* txt = "INTERRUPT 11!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler12()
{
    const char* txt = "INTERRUPT 12!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler13()
{
    const char* txt = "INTERRUPT 13!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler14()
{
    const char* txt = "INTERRUPT 14!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void handler15()
{
    const char* txt = "INTERRUPT 15!";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}

__attribute__((naked)) void defaulthandler()
{
    const char* txt = "INTERRUPT! DEFAULT HANDLER";
    uint8_t *screen = (uint8_t*)0xB8000;

    while(*txt != 0)
    {
        *screen++ = *txt++;
        *screen++ = 7;
    }

    while(1) {};
}
