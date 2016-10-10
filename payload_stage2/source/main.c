/*
*   main.c
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



const char payloads[4][32] = {
    "boot_si.bin",
    "boot.bin",
    "ShadowNAND/boot.bin",
    "homebrew/3ds/boot.bin"
};

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

u32 payloadIsOnSD(u32 index){
    u32 res;

    for (int i = 0; i < 4; ++i)
    {
        if(fileCheck(payloads[i])){
            if (index) // get file index
            {
                res = i;
            }
            else{ // get file availability
                res = 1; // if file exists but doesn't enforce SI
                if (i == 0)
                {
                    res = 2; // if file exists AND enforces SI
                }
            }
            break;
        }
    }
    return res;
}

void main(void)
{
    mountSD();

    u32 payloadFound;
    u32 payloadExists;
    u32 screenInit;

    if(fileRead((void *)PAYLOAD_ADDRESS, "a9nc.bin")) // Full A9NC support
    {
        payloadFound = 1;
        ownArm11(1);
        clearScreens();
        i2cWriteRegister(3, 0x22, 0x2A); //Turn on backlight
        f_unlink("a9nc.bin");
    }
    else
    {
        payloadExists = payloadIsOnSD(0); // check file availability
        if(payloadExists)
        {
            if(fileRead((void *)PAYLOAD_ADDRESS, payloads[payloadIsOnSD(1)])) // try to read the existing file in to the memory
            {
                payloadFound = 1;
                if (payloadExists == 2)
                {
                    screenInit = 1;
                }
                else if (fileCheck("ShadowNAND/screeninit")){
                    screenInit = 1;
                }
                else
                {
                    if (HID_PAD != BUTTON_LEFT) // If DPAD_LEFT is not held
                    {
                        screenInit = 0;
                    }
                    else // If DPAD_LEFT is held
                    {
                        screenInit = 1;
                    }
                }

                if (screenInit == 0)
                {
                    ownArm11(0); // Don't init the screen
                }

                if (screenInit == 1)// If DPAD_LEFT is held
                {
                    ownArm11(1); // Init the screen
                    clearScreens();
                    i2cWriteRegister(3, 0x22, 0x2A); //Turn on backlight
                }
            }
        }
        else //No payload found/no SD inserted
        {
            payloadFound = 0;
            ownArm11(0);
        }
    }

    //Jump to payload
    if(payloadFound)
    {
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