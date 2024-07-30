//
// Created by Anthony Lesch on 7/25/24.
//

#include <SDL.h>
#include <SDL_mixer.h>

#include "Audio.h"

#include <stdbool.h>

#define MIX_DEFAULT_CHUNK_SIZE 4096
#define DEFAULT_FREQUENCY 48000
#define MAX_SFX_COUNT 8

static bool audioInitialized;
static Mix_Music* music = NULL;
static Mix_Chunk** soundEffects = NULL;
static i8 currentEffectIndex = 0;

void AUD_InitializeAudioSystem(void)
{
    if (audioInitialized)
    {
        SDL_Log("Audio initialization can only occur once.");
        return;
    }

    audioInitialized = false;

    if (Mix_OpenAudio(DEFAULT_FREQUENCY, AUDIO_F32SYS, MIX_DEFAULT_CHANNELS, MIX_DEFAULT_CHUNK_SIZE) < 0)
    {
        SDL_Log("SDL_mixer could not open audio system! SDL_mixer Error: %s", Mix_GetError());
        return;
    }

    soundEffects = calloc(MAX_SFX_COUNT, sizeof(Mix_Chunk*));
    if (soundEffects == NULL)
    {
        SDL_Log("Could not allocated memory for the SFX buffer.");
        return;
    }

    audioInitialized = true;
}

void AUD_DestroyAudioSystem(void)
{
    if (!audioInitialized) return;

    audioInitialized = false;
    SDL_Log("Destroying audio system.");

    for (size_t i = 0; i < currentEffectIndex; ++i)
    {
        Mix_Chunk* sfx = soundEffects[i];
        if (sfx == NULL) continue;

        Mix_FreeChunk(sfx);
    }

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
        SDL_Log("Unloading music (%s)", Mix_GetMusicTitle(music));

        if (Mix_PlayingMusic())
        {
            Mix_FadeOutMusic(1500);
        }

        Mix_FreeMusic(music);
    }

    SDL_Log("Loading music at path: %s", filepath);

    music = Mix_LoadMUS(filepath);
    if (music == NULL)
    {
        SDL_Log("Unable to load music file (%s): %s", filepath, Mix_GetError());
        return;
    }

    if (Mix_FadeInMusic(music, -1, 1000))
    {
        SDL_Log("Unable to fade music in!: %s", Mix_GetError());
    }
}

void AUD_PlaySoundEffect(const i8 index)
{
    if (!audioInitialized) return;
    if (index < 0) return;

    Mix_Chunk* sfx = soundEffects[index];
    if (Mix_PlayChannel(-1, sfx, 0) == -1)
    {
        SDL_Log("Unable to play SFX: %s", Mix_GetError());
    }
}

i8 AUD_LoadSoundEffect(const char* filepath)
{
    if (!audioInitialized) return -1;
    if (currentEffectIndex >= MAX_SFX_COUNT) return -1;

    Mix_Chunk* sfx = Mix_LoadWAV(filepath);
    if (sfx == NULL)
    {
        SDL_Log("Unable to load SFX: %s", Mix_GetError());
        return -1;
    }

    soundEffects[currentEffectIndex] = sfx;

    return currentEffectIndex++;
}
