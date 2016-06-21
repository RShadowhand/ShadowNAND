/*
*   patches.h
*/

#pragma once

#include "types.h"

u8 *getProcess9(u8 *pos, u32 size, u32 *process9Size);
u32 patchSignatureChecks(u8 *pos, u32 size);
u32 patchFirmWrites(u8 *pos, u32 size);