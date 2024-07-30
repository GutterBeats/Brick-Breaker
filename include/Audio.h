//
// Created by Anthony Lesch on 7/25/24.
//

#pragma once

#include "Types.h"

void AUD_InitializeAudioSystem(void);
void AUD_DestroyAudioSystem(void);

void AUD_PlayMusic(const char* filepath);
void AUD_PlaySoundEffect(i8 index);
i8 AUD_LoadSoundEffect(const char* filepath);
