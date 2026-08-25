/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef RETROACHIEVEMENTS_H
#define RETROACHIEVEMENTS_H

#include <stddef.h>

void RA_Initialize(void);
void RA_Shutdown(void);
void RA_OnRomLoaded(const unsigned char* rom, size_t rom_size);
void RA_OnRomClosed(void);
void RA_DoFrame(void);
void RA_Idle(void);
void RA_OnLoadState(void);
void RA_SetMemoryTable(const unsigned char** table);

#endif
