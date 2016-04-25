#include "types.h"
#include "i2c.h"
#include "screen.h"
#include "fatfs/ff.h"

#define PAYLOAD_ADDRESS 0x23F00000
#define A11_PAYLOAD_LOC 0x1FFF4C80 //keep in mind this needs to be changed in the ld script for arm11 too
#define A11_ENTRY       0x1FFFFFF8

extern u8 arm11_bin[];
extern u32 arm11_bin_size;

static void ownArm11()
{
    memcpy((void*)A11_PAYLOAD_LOC, arm11_bin, arm11_bin_size);
    *(vu32 *)A11_ENTRY = 1;
    *(vu32 *)0x1FFAED80 = 0xE51FF004;
    *(vu32 *)0x1FFAED84 = A11_PAYLOAD_LOC;
    *(vu8 *)0x1FFFFFF0 = 2;
    while(*(vu32 *)A11_ENTRY);
}

void main()
{
    FATFS fs;
    FIL payload;
    unsigned int br;

    f_mount(&fs, "0:", 0); //This never fails due to deferred mounting
    if(f_open(&payload, "safe_mode.bin", FA_READ) == FR_OK)
    {
        setFramebuffers();
        ownArm11();
        clearScreens();
        turnOnBacklight();
        f_read(&payload, (void *)PAYLOAD_ADDRESS, f_size(&payload), &br);
        ((void (*)())PAYLOAD_ADDRESS)();
    }

    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1);
}
