/*

    Create a multiboot header

*/

#include <stdint.h>


typedef struct
{
    uint32_t mb_magic;      // multiboot magic number
    uint32_t mb_arch;       // architecture id
    uint32_t mb_length;     // length of multiboot header
    uint32_t mb_checksum;   // checksum
    uint32_t tag1;
    uint32_t tag2;
} MultibootHeader_t;

MultibootHeader_t header __attribute__ ((section("boot"))) = 
{
    0xe85250d6,
    0,
    sizeof(MultibootHeader_t),
    0x100000000 - (0xe85250d6 + sizeof(MultibootHeader_t)),
    0,
    8
};

