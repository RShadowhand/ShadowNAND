#include "types.h"
#include "buttons.h"
#include "sdmmc.h"

void main()
{
    // Initialize sdcard and nand
    sdmmc_sdcard_init();

    u32 pressed = HID_PAD;

    if(pressed == BUTTON_DOWN) //if DPAD_DOWN is pressed
    {
        //Jump to alternate stage2
        sdmmc_nand_readsectors(0x5A000, 0x64, (u8*)0x08006000);
    }
    else
    {
        //jump to normal stage2
        sdmmc_nand_readsectors(0x5C000, 0x64, (u8*)0x08006000);
    }

    // Jump to secondary payload
    ((void (*)())0x08006000)();
}
