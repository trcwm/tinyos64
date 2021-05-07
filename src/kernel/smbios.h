/*

    SMBios stuff
    https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.4.0.pdf

*/

#pragma once

#include <stdint.h>

#pragma pack(push, 2)
typedef struct
{
    const char  magic[4];    // should be _SM_
    uint8_t     chksum;
    uint8_t     version_major;
    uint8_t     version_minor;
    uint16_t    max_struct_size;    // max structure size in bytes
    uint8_t     entry_point_revision;
    uint8_t     formatted_area[5];
    const char  magic2[5];  // should be _DMI_
    uint8_t     checksum2;
    uint16_t    struct_tbl_len;     // total length of SMBIOS structure table, bytes.
    uint32_t    struct_tbl_address; // physical 32-bit address.
    uint16_t    num_structs;        // number of structures
    uint8_t     bcd_revision;       // (optional)
} SMBios32Header_t;

typedef struct
{
    const char  magic[5];    // should be _SM3_
    uint8_t     chksum;
    uint8_t     entry_point_length; // Length of the Entry Point Structure, starting with the Anchor String field, in bytes, currently 18h
    uint8_t     version_major;
    uint8_t     version_minor;
    uint8_t     smbios_docrev;     
    uint8_t     entry_point_revision;
    uint8_t     reserved;
    uint32_t    struct_tbl_max_size;    //Structure table maximum size in bytes
    uint64_t    struct_tbl_address;
} SMBios64Header_t;

typedef struct
{
    uint8_t     stype;  // structure type
    uint8_t     len;    // length in bytes of formatted structure area
    uint16_t    handle; // structure handle
} SMStructHeader_t;

// type 0 structure
typedef struct
{
    uint8_t     vendorStrId;
    uint8_t     biosVerStrId;
    uint16_t    biosStartSegment;
    uint8_t     biosDateStrId;
    uint8_t     biosROMSize;            // blocks of 64K
    uint64_t    biosCharacteristics;    // bitfield
    uint8_t     biosCharacteristicsExt[2];  // bitfield extension bytes
} SMBiosInfoFormatted_t;

#pragma pack(pop)

#include <stdbool.h>
void* detect_smbios();
