# ShadowNAND
*CFW in your NAND.*

##How to Install
* Download **ShadowNAND Installer** from [here](https://github.com/RShadowhand/ShadowNAND_Installer/releases/latest)
* Download **ShadowNAND** itself from [here](https://github.com/RShadowhand/ShadowNAND/releases/latest)
* Download other required files from [here](https://mega.co.nz/#!RwUDVL5T!65gKJHAAVFk3R0jCA7zRFC5q5QTsL5CLoRUoqhET-WI)
* Put ShadowNAND files, other required files, and your `otp.bin` to `SD:\homebrew\a9lh\`
* Boot from `ShadowNAND_Installer.bin` or other entry points.
* Press `SELECT`
* Done.

##What can it do:
* Boot `homebrew/boot.bin`, this can be any payload.
* Boot `homebrew/boot.bin` with screeninit by pressing `DPAD_LEFT` on launch.
* Boot `homebrew/safe_mode.bin` by pressing `DPAD_DOWN` on launch, this can be any payload.
* Boot to SysNAND if no payload or SD found. (thanks to @AuroraWright)

##SysNAND booting:
* Patches firmwrites, so you can update your sysNAND.
* Patches sigchecks so it can show/launch unsigned programs.
* No reboot patches yet, so no AGB/TWL/SAFE_MODE booting.

##Planned features:

* Splash screen.
* Reboot patches.
* AGB/TWL FIRM patches.
* Built-in bootloader with GUI.
* EmuNAND support.
* External injector/loader support.
* Region/Language emulation.
* Region-free patches.
* `firmware.bin` support.

## Credits

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
* AuroraWright for the main features.
