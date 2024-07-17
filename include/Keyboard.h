//
// Created by Anthony Lesch on 7/15/24.
//

#pragma once

#include <SDL.h>

enum KBD_KEY_STATE
{
    KEY_STATE_UP,
    KEY_STATE_DOWN
};

typedef struct keymap
{
    SDL_Scancode Left;
    SDL_Scancode Right;
    SDL_Scancode Pause;
} KeyMap;

typedef struct keyState
{
    enum KBD_KEY_STATE Left;
    enum KBD_KEY_STATE Right;
} KeyState;

void KBD_InitializeKeymap();

void KBD_HandleEvent(const SDL_Event* event);

SDL_bool KBD_IsLeftKeyDown();

SDL_bool KBD_IsRightKeyDown();
