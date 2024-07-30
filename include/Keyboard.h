//
// Created by Anthony Lesch on 7/15/24.
//

#pragma once

#include <SDL.h>
#include <stdbool.h>

typedef enum USER_EVENT_TYPE
{
    PAUSE,
    ENTER,
    DEBUG
} USER_EVENT_TYPE;

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
    SDL_Scancode Enter;
    SDL_Scancode Down;
    SDL_Scancode Up;
    SDL_Scancode Debug;
} KeyMap;

typedef struct keyState
{
    enum KBD_KEY_STATE Left;
    enum KBD_KEY_STATE Right;
    enum KBD_KEY_STATE Up;
    enum KBD_KEY_STATE Down;
} KeyState;

void KBD_InitializeKeymap();

void KBD_HandleEvent(const SDL_Event* event);

bool KBD_IsLeftKeyDown();
bool KBD_IsRightKeyDown();
