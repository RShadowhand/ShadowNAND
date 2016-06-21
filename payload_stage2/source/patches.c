/*
*   patches.c
*/

#include "patches.h"
#include "memory.h"

u8 *getProcess9(u8 *pos, u32 size, u32 *process9Size)
{
    u8 *off = memsearch(pos, "ess9", size, 4);

    *process9Size = *(u32 *)(off - 0x60) * 0x200;

    //Process9 code offset (start of NCCH + ExeFS offset + ExeFS header size)
    return off - 0x204 + (*(u32 *)(off - 0x64) * 0x200) + 0x200;
}

u32 patchSignatureChecks(u8 *pos, u32 size)
{
    const u16 sigPatch[2] = {0x2000, 0x4770};

    //Look for signature checks
    const u8 pattern[] = {0xC0, 0x1C, 0x76, 0xE7},
             pattern2[] = {0xB5, 0x22, 0x4D, 0x0C};

    u16 *off = (u16 *)memsearch(pos, pattern, size, 4);
    u8 *off2Tmp = memsearch(pos, pattern2, size, 4);

    if(off == NULL || off2Tmp == NULL) return 1;
    
    u16 *off2 = (u16 *)(off2Tmp - 1);

    *off = sigPatch[0];
    off2[0] = sigPatch[0];
    off2[1] = sigPatch[1];

    return 0;
}

u32 patchFirmWrites(u8 *pos, u32 size)
{
    const u16 writeBlock[2] = {0x2000, 0x46C0};

    //Look for FIRM writing code
    u8 *const off1 = memsearch(pos, "exe:", size, 4);

    if(off1 == NULL) return 1;

    const u8 pattern[] = {0x00, 0x28, 0x01, 0xDA};

    u16 *off2 = (u16 *)memsearch(off1 - 0x100, pattern, 0x100, 4);

    if(off2 == NULL) return 1;

    off2[0] = writeBlock[0];
    off2[1] = writeBlock[1];

    return 0;
}