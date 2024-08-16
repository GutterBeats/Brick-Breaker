//
// Created by Anthony Lesch on 7/25/24.
//

#pragma once

#include "Types.h"

void AUD_InitializeAudioSystem(void);
void AUD_DestroyAudioSystem(void);

void AUD_PlayMusic(const char* filepath);
void AUD_ResumeMusic(void);
void AUD_PauseMusic(void);
void AUD_PlaySoundEffect(i8 index);
i8 AUD_LoadSoundEffect(const char* filepath);
void AUD_UnloadSoundEffect(i8 index);
void AUD_SetMusicVolume(u8 volume);
void AUD_SetSFXVolume(u8 volume);
