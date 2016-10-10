/*
*   fs.h
*/

#pragma once

#include "types.h"

extern u32 console;

void mountSD(void);
void unmountSD(void);
u32 mountCTRNAND(void);
u32 fileRead(void *dest, const char *path);
void firmRead(void *dest);