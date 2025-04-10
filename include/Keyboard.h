//
// Created by Anthony Lesch on 7/15/24.
//

#pragma once

#include <SDL.h>
#include <stdbool.h>

typedef enum USER_EVENT_TYPE
{
    // First index of user event. Make sure is always first.
    USER_EVENT_NONE = -1,

    ENTER = 0,
    DEBUG,
    PAUSE,
    CLOSE,
    UP_KEY_PRESSED,
    DOWN_KEY_PRESSED,

    // Count of the user event types. Make sure this is always last.
    USER_EVENT_COUNT
} USER_EVENT_TYPE;

#define IS_VALID_USER_EVENT(x) ((x > USER_EVENT_NONE) && (x < USER_EVENT_COUNT))

typedef struct key_array
{
    uint8_t Count;

    SDL_Keycode* Keys;
} KeyArray;

typedef struct keymap
{
    KeyArray* Left;
    KeyArray* Right;
    KeyArray* Down;
    KeyArray* Up;
    KeyArray* Enter;
    SDL_Keycode Pause;
    SDL_Keycode Debug;
    SDL_Keycode Close;
} KeyMap;

void KBD_InitializeKeymap(void);
void KBD_DestroyKeymap(void);

void KBD_HandleEvent(const SDL_Event* event);

bool KBD_IsLeftKeyDown(void);
bool KBD_IsRightKeyDown(void);
