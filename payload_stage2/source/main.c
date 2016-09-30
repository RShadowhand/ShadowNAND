/*
*   main.c
*/


/*
 * DNANwodahS by gnmmarechal
 * 
 * Fork of ShadowNAND
 * 
 * Licensed under the GPLv2
 * 
 * You can use ShadowNAND Installer to install DNANwodahS
 * 
 * Base ShadowNAND Version: v0.85
 * 
 * DNANwodahS Version: v0.2
 * 
 */
 
 
#include "types.h"
#include "buttons.h"
#include "memory.h"
#include "i2c.h"
#include "cache.h"
#include "fs.h"
#include "firm.h"
#include "../build/bundled.h"

#define A11_PAYLOAD_LOC 0x1FFF4C80 //Keep in mind this needs to be changed in the ld script for arm11 too
#define A11_ENTRY       0x1FFFFFF8

static void ownArm11(u32 screenInit)
{
    memcpy((void *)A11_PAYLOAD_LOC, arm11_bin, arm11_bin_size);

    //Let the ARM11 code know if it needs to screen init
    *(vu32 *)(A11_PAYLOAD_LOC + 8) = screenInit;

    *(vu32 *)A11_ENTRY = 1;
    *(vu32 *)0x1FFAED80 = 0xE51FF004;
    *(vu32 *)0x1FFAED84 = A11_PAYLOAD_LOC;
    *(vu8 *)0x1FFFFFF0 = 2;
    while(*(vu32 *)A11_ENTRY);
}

static inline void clearScreens(void)
{
    memset32((void *)0x18300000, 0, 0x46500);
    memset32((void *)0x18346500, 0, 0x38400);
}

void main(void)
{
    mountSD();

    u32 payloadFound;
    u32 defaultInit;
    u32 skipInit = 0;

    if(fileRead((void *)PAYLOAD_ADDRESS, "homebrew/3ds/a9nc.bin")) // Full A9NC support
    {
        payloadFound = 1;
        skipInit = 1;
        ownArm11(1);
        clearScreens();
        i2cWriteRegister(3, 0x22, 0x2A); //Turn on backlight
        f_unlink("homebrew/3ds/a9nc.bin");
    }
    else if(fileRead((void *)PAYLOAD_ADDRESS, "arm9loaderhax.bin")) // Boots from /arm9loaderhax.bin, by default screeninit is off, can be turned on with a keypress at boot
    {
        payloadFound = 1;
        defaultInit = 0;
    }
    else if(fileRead((void *)PAYLOAD_ADDRESS, "arm9loaderhax_si.bin")) // Boots from /arm9loaderhax_si.bi, screeninit is on by default.
    {
        payloadFound = 1;
        defaultInit = 1;
    }    
    else if(fileRead((void *)PAYLOAD_ADDRESS, "homebrew/3ds/boot.bin")) // Boots from ShadowNAND's standard boot.bin, screeninit is off by default.
    {
		payloadFound = 1;
		defaultInit = 0;
	}    
    else if(fileRead((void *)PAYLOAD_ADDRESS, "homebrew/3ds/boot_si.bin"))// Boots from /homebrew/3ds/boot_si.bin, screeninit is on by default. 
    {
		payloadFound = 1;
		defaultInit = 1;
	}
    else //No payload found/no SD inserted
    {
        payloadFound = 0;
        ownArm11(0);
    }

    //Jump to payload
    if(payloadFound)
    {
		if(!skipInit)
		{
			if(defaultInit)
			{
				if (HID_PAD != BUTTON_LEFT)
				{
					ownArm11(1);
					clearScreens();
					i2cWriteRegister(3, 0x22, 0x2A);
				}
				else
				{
					ownArm11(0);
				}
			}
			else
			{
				if (HID_PAD == BUTTON_LEFT)
				{
					ownArm11(1);
					clearScreens();
					i2cWriteRegister(3, 0x22, 0x2A);
				}
				else
				{
					ownArm11(0);
				}				
			}		
		}
        flushCaches();

        ((void (*)())PAYLOAD_ADDRESS)();
    }

    unmountSD();

    //If the SAFE_MODE combo is not pressed, try to patch and boot the CTRNAND FIRM
    if(HID_PAD != SAFE_MODE) loadFirm();

    flushCaches();
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1);
    while(1);
}
