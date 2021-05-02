// https://www.millisecond.com/support/docs/v6/html/language/scancodes.htm

#include <stdbool.h>
#include "keyboard.h"
#include "ports.h"

/* Standard and AT keyboard.  (PS/2 MCA implies AT throughout.) */
#define KEYBD 0x60 /* I/O port for keyboard data */

/* AT keyboard. */
#define KB_COMMAND 0x64     /* I/O port for commands on AT */
#define KB_STATUS 0x64      /* I/O port for status on AT */
#define KB_ACK 0xFA         /* keyboard ack response */
#define KB_AUX_BYTE 0x20    /* Auxiliary Device Output Buffer Full */
#define KB_OUT_FULL 0x01    /* status bit set when keypress char pending */
#define KB_IN_FULL 0x02     /* status bit set when not ready to receive */
#define KBC_RD_RAM_CCB 0x20 /* Read controller command byte */
#define KBC_WR_RAM_CCB 0x60 /* Write controller command byte */
#define KBC_DI_AUX 0xA7     /* Disable Auxiliary Device */
#define KBC_EN_AUX 0xA8     /* Enable Auxiliary Device */
#define KBC_DI_KBD 0xAD     /* Disable Keybard Interface */
#define KBC_EN_KBD 0xAE     /* Enable Keybard Interface */
#define LED_CODE 0xED       /* command to keyboard to set LEDs */

#define KBC_WAIT_TIME 100000  /* wait this many usecs for a status update */
#define KBC_READ_TIME 1000000 /* wait this many usecs for a result byte */

#define KBC_IN_DELAY 7 /* wait 7 microseconds when polling */

#define KBD_OUT_BUFSZ 16 /* Output buffer for data to the keyboard. */

#define KBD_SCAN_CODES 0x80

#define SCAN_RELEASE 0x80
#define SCAN_CTRL 0x1D
#define SCAN_NUMLOCK 0x45
#define SCAN_EXT0 0xE0
#define SCAN_EXT1 0xE1

#define LED_SCROLL_LOCK 0x01
#define LED_NUM_LOCK 0x02
#define LED_CAPS_LOCK 0x04

static uint8_t scancodes[256] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    // 0x00 - 0x0F
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    // 0x00 - 0x0F
};

static bool kb_wait()
{
    while(true)
    {
        uint8_t status = inb(KB_STATUS);
        if (status & KB_IN_FULL)
        {
            return true;
        }
    }
}

uint8_t keyboard_scan()
{
    // wait for keyboard to have data..
    uint8_t status = inb(KB_STATUS);
    while((status & KB_OUT_FULL) == 0) 
    {
        status = inb(KB_STATUS);
    };  

    // delay a bit
    for(uint32_t i=0; i<1000; i++) {};     // wait a bit

    // get response
    return inb(KEYBD);
}

//#define KBFLAG_SHIFT 1
//static uint32_t kb_flags = 0;

static const uint8_t scan2ascii[128] =
{
    0,      // 0 - no key
    0,      // 1 - ESCAPE
    '1',    // 2 - numeric 1
    '2',    // 3 - numeric 2
    '3',    // 4 - numeric 3
    '4',    // 5 - numeric 4
    '5',    // 6 - numeric 5
    '6',    // 7 - numeric 6
    '7',    // 8 - numeric 7
    '8',    // 9 - numeric 8
    '9',    // 10 - numeric 9
    '0',    // 11 - numeric 0
    '-',    // 12
    '=',    // 13
    0,      // backspace
    '\t',   // tab
    'q',    // 16
    'w',    // 17
    'e',    // 18
    'r',    // 19
    't',    // 20
    'y',    // 21
    'u',    // 22
    'i',    // 23
    'o',    // 24
    'p',    // 25
    '[',    // 26
    ']',    // 27
    '\n',   // 28 ENTER
    0,      // 29 CONTROL
    'a',    // 30 
    's',    // 31 
    'd',    // 32 
    'f',    // 33 
    'g',    // 34 
    'h',    // 35 
    'j',    // 36 
    'k',    // 37 
    'l',    // 38 
    ';',    // 39 
    '\'',   // 40 
    '`',    // 41 
    0,      // 42, Left Shift 
    '\\',   // 43
    'z',    // 44
    'x',    // 45
    'c',    // 46
    'v',    // 47
    'b',    // 48
    'n',    // 49
    'm',    // 50
    ',',    // 51
    '.',    // 52
    '/',    // 53
    0,      // 54, Right Shift
    0,      // 55, PrtSc
    0,      // 56, Alt
    ' ',    // 57, Space
    0,      // 58, Caps Lock
    0,      // 59, F1
    0,      // 60, F1
    0,      // 61, F1
    0,      // 62, F1
    0,      // 63, F1
    0,      // 64, F1
    0,      // 65, F1
    0,      // 66, F1
    0,      // 67, F1
    0,      // 68, F1
    0,      // 69, F1
    0,      // 70, F1
    0,      // 71, F1
    0,      // 72, F1
    0,      // 73, F1
    '-',    // 74, keypad
    0,      // 75, Left
    0,      // 76, Center
    0,      // 77, Right
    '+',    // 78,
    0,      // 79, End
    0,      // 80, Down
    0,      // 81, PgDn
    0,      // 82, Ins
    0,      // 83, Del
};

char keyboard_read()
{
    char c = 0;
    while(c == 0)
    {
        uint8_t code = keyboard_scan();
        if ((code & 0x80) == 0)
        {
            // ignore release
            c = scan2ascii[code];
        }
    }
    return c;
}
