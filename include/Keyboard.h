//
// Created by Anthony Lesch on 7/15/24.
//

#pragma once

#include <SDL.h>
#include <stdbool.h>

typedef enum USER_EVENT_TYPE
{
    ENTER,
    DEBUG,
    PAUSE
} USER_EVENT_TYPE;

enum KBD_KEY_STATE
{
    KEY_STATE_UP,
    KEY_STATE_DOWN
};

typedef struct key_array
{
    uint8_t Count;

    SDL_Scancode* Keys;
} KeyArray;

typedef struct keymap
{
    KeyArray* Left;
    KeyArray* Right;
    KeyArray* Down;
    KeyArray* Up;
    KeyArray* Enter;
    SDL_Scancode Pause;
    SDL_Scancode Debug;
} KeyMap;

typedef struct keyState
{
    enum KBD_KEY_STATE Left;
    enum KBD_KEY_STATE Right;
    enum KBD_KEY_STATE Up;
    enum KBD_KEY_STATE Down;
} KeyState;

void KBD_InitializeKeymap(void);
void KBD_DestroyKeymap(void);

void KBD_HandleEvent(const SDL_Event* event);

bool KBD_IsLeftKeyDown(void);
bool KBD_IsRightKeyDown(void);
