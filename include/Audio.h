//
// Created by Anthony Lesch on 7/25/24.
//

#pragma once

#include "Types.h"

void InitializeAudioSystem(void);
void DestroyAudioSystem(void);

void PlayMusic(const char* filepath);
void PlaySoundEffect(u8 index);
u8 LoadSoundEffect(const char* filepath);
