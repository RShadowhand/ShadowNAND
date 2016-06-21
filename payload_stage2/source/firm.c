/*
*   firm.c
*/

#include "firm.h"
#include "fs.h"
#include "memory.h"
#include "crypto.h"
#include "patches.h"
#include "cache.h"
#include "../build/bundled.h"

static firmHeader *const firm = (firmHeader *)0x24000000;
static const firmSectionHeader *section;

u32 console;

static inline u32 patchFirm(void)
{
    u8 *arm9Section = (u8 *)firm + section[2].offset;

    if(console)
    {
        //Decrypt ARM9Bin and patch ARM9 entrypoint to skip arm9loader
        arm9Loader(arm9Section);
        firm->arm9Entry = (u8 *)0x801B01C;
    }

    //Find the Process9 .code location, size and memory address
    u32 process9Size;
    u8 *process9Offset = getProcess9(arm9Section + 0x15000, section[2].size - 0x15000, &process9Size);

    u32 ret = 0;

    //Apply signature patches
    ret += patchSignatureChecks(process9Offset, process9Size);

    //Apply FIRM0/1 writes patches on sysNAND to protect A9LH
    ret += patchFirmWrites(process9Offset, process9Size);

    return ret;
}

static inline void launchFirm(void)
{
    memcpy((void *)PAYLOAD_ADDRESS, launcher_bin, launcher_bin_size);

    flushCaches();

    ((void (*)())PAYLOAD_ADDRESS)();
}

void loadFirm(void)
{
    //Detect the console being used
    console = PDN_MPCORE_CFG == 7;

    if(mountCTRNAND()) return;

    firmRead(firm);
    decryptExeFs((u8 *)firm);

    section = firm->section;

    if(!patchFirm()) launchFirm();
}