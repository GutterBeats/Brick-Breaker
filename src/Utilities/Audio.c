//
// Created by Anthony Lesch on 7/25/24.
//

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdbool.h>

#include "Audio.h"
#include "Utils.h"

#define MIX_DEFAULT_CHUNK_SIZE 1024
#define DEFAULT_FREQUENCY 48000
#define MAX_SFX_COUNT 8

static bool audioInitialized;
static Mix_Music* music = NULL;
static Mix_Chunk** soundEffects = NULL;

void AUD_InitializeAudioSystem(void)
{
    if (audioInitialized)
    {
        BB_LOG("Audio initialization can only occur once.");
        return;
    }

    audioInitialized = false;

    if (Mix_OpenAudio(DEFAULT_FREQUENCY, AUDIO_F32SYS, MIX_DEFAULT_CHANNELS, MIX_DEFAULT_CHUNK_SIZE) < 0)
    {
        BB_LOG("SDL_mixer could not open audio system! SDL_mixer Error: %s", Mix_GetError());
        return;
    }

    soundEffects = calloc(MAX_SFX_COUNT, sizeof(Mix_Chunk*));
    if (soundEffects == NULL)
    {
        BB_LOG("Could not allocate memory for the SFX buffer.");
        return;
    }

    audioInitialized = true;
}

void AUD_DestroyAudioSystem(void)
{
    if (!audioInitialized) return;

    audioInitialized = false;
    BB_LOG("Destroying audio system.");

    for (size_t i = 0; i < MAX_SFX_COUNT; ++i)
    {
        Mix_Chunk* sfx = soundEffects[i];
        if (sfx == NULL) continue;

        Mix_FreeChunk(sfx);
    }

    free(soundEffects);

    Mix_FadeOutMusic(1000);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
}

void AUD_PlayMusic(const char* filepath)
{
    if (!audioInitialized) return;

    if (music != NULL)
    {
        BB_LOG("Unloading music (%s)", Mix_GetMusicTitle(music));

        if (Mix_PlayingMusic())
        {
            Mix_FadeOutMusic(1500);
        }

        Mix_FreeMusic(music);
    }

    BB_LOG("Loading music at path: %s", filepath);

    music = Mix_LoadMUS(filepath);
    if (music == NULL)
    {
        BB_LOG("Unable to load music file (%s): %s", filepath, Mix_GetError());
        return;
    }

    if (Mix_FadeInMusic(music, -1, 1000))
    {
        BB_LOG("Unable to fade music in!: %s", Mix_GetError());
    }

    AUD_SetMusicVolume(65);
}

void AUD_ResumeMusic(void)
{
    if (music == NULL) return;
    if (!Mix_PausedMusic()) return;

    Mix_ResumeMusic();
}

void AUD_PauseMusic(void)
{
    if (music == NULL) return;
    if (Mix_PausedMusic()) return;

    Mix_PauseMusic();
}

void AUD_PlaySoundEffect(const i8 index)
{
    if (!audioInitialized) return;
    if (index < 0) return;

    Mix_Chunk* sfx = soundEffects[index];
    if (Mix_PlayChannel(-1, sfx, 0) == -1)
    {
        BB_LOG("Unable to play SFX: %s", Mix_GetError());
    }
}

i8 AUD_LoadSoundEffect(const char* filepath)
{
    if (!audioInitialized) return -1;

    for (i8 i = 0; i < MAX_SFX_COUNT; ++i)
    {
        if (soundEffects[i] != NULL) continue;

        Mix_Chunk* sfx = Mix_LoadWAV(filepath);
        if (sfx == NULL)
        {
            BB_LOG("Unable to load SFX: %s", Mix_GetError());
            return -1;
        }

        soundEffects[i] = sfx;
        return i;
    }

    BB_LOG("Unable to load sound effect (%s). No free slots available.", filepath);

    return -1;
}

void AUD_UnloadSoundEffect(const i8 index)
{
    if (index > MAX_SFX_COUNT || index < 0) return;

    Mix_Chunk* sfx = soundEffects[index];
    if (sfx == NULL) return;

    Mix_FreeChunk(sfx);
    soundEffects[index] = NULL;
}

void AUD_SetMusicVolume(const u8 volume)
{
    Mix_VolumeMusic(UTL_Clamp(0, MIX_MAX_VOLUME, volume));
}

void AUD_SetSFXVolume(const u8 volume)
{
    for (i8 i = 0; i < MAX_SFX_COUNT; ++i)
    {
        Mix_Chunk* sfx = soundEffects[i];
        if (sfx == NULL) continue;

        Mix_VolumeChunk(sfx, UTL_Clamp(0, MIX_MAX_VOLUME, volume));
    }
}
