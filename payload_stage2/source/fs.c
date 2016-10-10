/*
*   fs.c
*/

#include "fs.h"
#include "memory.h"
#include "fatfs/ff.h"

static FATFS fs;

void mountSD(void)
{
    f_mount(&fs, "0:", 0);
}

void unmountSD(void)
{
    f_mount(NULL, "0:", 1);
}

u32 mountCTRNAND(void)
{
    return f_mount(&fs, "1:", 1) != FR_OK;
}

u32 fileCheck(const char *path)
{
    FIL file;
    u32 size;

    if(f_open(&file, path, FA_READ) == FR_OK)
    {
        size = f_size(&file);
        f_close(&file);
    }
    else size = 0;

    return size;
}

u32 fileRead(void *dest, const char *path)
{
    FIL file;
    u32 size;

    if(f_open(&file, path, FA_READ) == FR_OK)
    {
        unsigned int read;
        size = f_size(&file);
        f_read(&file, dest, size, &read);
        f_close(&file);
    }
    else size = 0;

    return size;
}

void firmRead(void *dest)
{
    const char *firmFolders[] = { "00000002", "20000002" };
    char path[48] = "1:/title/00040138/00000000/content";
    memcpy(&path[18], firmFolders[console], 8);

    DIR dir;
    FILINFO info;

    f_opendir(&dir, path);

    u32 id = 0xFFFFFFFF;

    //Parse the target directory
    while(f_readdir(&dir, &info) == FR_OK && info.fname[0])
    {
        //Not a cxi
        if(info.altname[9] != 'A') continue;

        //Convert the .app name to an integer
        u32 tempId = 0;
        for(char *tmp = info.altname; *tmp != '.'; tmp++)
        {
            tempId <<= 4;
            tempId += *tmp > '9' ? *tmp - 'A' + 10 : *tmp - '0';
        }

        //Found an older cxi
        if(tempId < id) id = tempId;
    }

    f_closedir(&dir);

    //Complete the string with the .app name
    memcpy(&path[34], "/00000000.app", 14);

    //Last digit of the .app
    u32 i = 42;

    //Convert back the .app name from integer to array
    while(id)
    {
        static const char hexDigits[] = "0123456789ABCDEF";
        path[i--] = hexDigits[id & 0xF];
        id >>= 4;
    }

    fileRead(dest, path);
}