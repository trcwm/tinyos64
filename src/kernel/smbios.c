/*

    SMBios stuff
    https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.4.0.pdf

*/

#include <stdint.h>
#include "smbios.h"

void* detect_smbios()
{
    // search for SMbios signature _sm_ or _sm3 on 16-byte boundaries 
    // from 0xF0000 ot 0xFFFFF

    uint32_t *ptr = (uint32_t*)0xF0000;
    while(ptr < (const uint32_t*)0xFFFFF)
    {
        if (*ptr == 0x5F4D535F /* _SM_ */)
            return (void*)ptr;
        
        if (*ptr == 0x5F4D5333 /* _SM3 */)
            return (void*)ptr;

        ptr += 16/sizeof(uint32_t);
    }

    return 0;
}
