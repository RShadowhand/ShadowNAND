# arm9loaderhax for 3DS

## What this is

This is my personal implementation of the arm9loaderhax exploit, documented [here](http://3dbrew.org/wiki/3DS_System_Flaws) and also presented [in this conference](https://media.ccc.de/v/32c3-7240-console_hacking), which provides ARM9 code execution directly at the console boot, exploiting a vulnerability present in 9.6+ version of New3DS arm9loader.

It works on both New and OLD 3DS.

This exploit was found by **plutoo** and **yellows8**, i do not own the idea.

## Usage

It attempts to load `bootmgr.bin` arm9 payload from the root of the sdcard at address 0x23F00000.  
If that fails, it'll try to load `arm9loaderhax.bin`.  
If neither of those files are found, it will shutdown the device.  
  
Pressing the safe mode keys (`L+R+Up+A`) on boot will go to alternate stage2, which will attempt to boot `safe_mode.bin` from your SD.  
If that doesn't exist, it'll shutdown the device.

## Installation

_(TODO)_

## Setup

_(TODO)_

## Credits

Copyright 2016, Jason Dellaluce

sdmmc.c & sdmmc.h originally written by Normatt

Licensed under GPLv2 or any later version, refer to the license.txt file included.

* Smealum and contributors for libctru
* Normatt for sdmmc.c and .h, and also for .ld files and the log from 3dmoo9 that provided us with some of the information needed to get screen init
* Christophe Devine for the SHA codes
* Archshift for i2c.c and .h
* Megazig for crypto.c and .h
* Patois for original BRAHMA code
* Smealum, Derrek, Plutoo for publishing the exploit
* Yellows8 and Plutoo as ideators of it
* [3dbrew community](http://3dbrew.org/)
* bilis/b1l1s for his screen init code, and work on inegrating it into stage 2
* dark_samus for work on integrating screen init into stage 2
* AuroraWright for buttons.h