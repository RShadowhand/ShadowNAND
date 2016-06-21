#include "firm.h"
#include "memory.h"
#include "cache.h"

void main(void)
{
    firmHeader *const firm = (firmHeader *)0x24000000;
    const firmSectionHeader *section = firm->section;

    //Copy FIRM sections to respective memory locations
    for(u32 i = 0; i < 3; i++)
        memcpy(section[i].address, (u8 *)firm + section[i].offset, section[i].size);

    flushCaches();

    *(vu32 *)0x1FFFFFF8 = (u32)firm->arm11Entry;
    ((void (*)())firm->arm9Entry)();
}