/*
*   buttons.h
*/

#pragma once

#include "types.h"

#define HID_PAD                (*(vu32 *)0x10146000 ^ 0xFFF)

#define BUTTON_R1              (1 << 8)
#define BUTTON_L1              (1 << 9)
#define BUTTON_A               1
#define BUTTON_UP              (1 << 6)
#define BUTTON_LEFT            (1 << 5)

#define SAFE_MODE              (BUTTON_R1 | BUTTON_L1 | BUTTON_A | BUTTON_UP)