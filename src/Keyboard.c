//
// Created by Anthony Lesch on 7/15/24.
//

#include "Keyboard.h"

static KeyMap s_KeyMap;
static KeyState s_KeyState;

static void HandleRealtimeKeys(SDL_Scancode scancode, enum KBD_KEY_STATE state);

void KBD_InitializeKeymap()
{
    s_KeyMap.Left = SDL_SCANCODE_A;
    s_KeyMap.Right = SDL_SCANCODE_D;
    s_KeyMap.Up = SDL_SCANCODE_UP;
    s_KeyMap.Down = SDL_SCANCODE_DOWN;
    s_KeyMap.Enter = SDL_SCANCODE_SPACE;
    s_KeyMap.Debug = SDL_SCANCODE_1;

    s_KeyState.Left = KEY_STATE_UP;
    s_KeyState.Right = KEY_STATE_UP;
    s_KeyState.Up = KEY_STATE_UP;
    s_KeyState.Down = KEY_STATE_UP;
}

void KBD_HandleEvent(const SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN:
            HandleRealtimeKeys(event->key.keysym.scancode, KEY_STATE_DOWN);
            break;
        case SDL_KEYUP:
            HandleRealtimeKeys(event->key.keysym.scancode, KEY_STATE_UP);
            break;
        default:
            break;
    }
}

bool KBD_IsLeftKeyDown()
{
    return s_KeyState.Left == KEY_STATE_DOWN;
}

bool KBD_IsRightKeyDown()
{
    return s_KeyState.Right == KEY_STATE_DOWN;
}

static void HandleRealtimeKeys(const SDL_Scancode scancode, const enum KBD_KEY_STATE state)
{
    if (s_KeyMap.Left == scancode)
    {
        s_KeyState.Left = state;
    }
    else if (s_KeyMap.Right == scancode)
    {
        s_KeyState.Right = state;
    }

    if (s_KeyMap.Up == scancode)
    {
        s_KeyState.Up = state;
    }
    else if (s_KeyMap.Down == scancode)
    {
        s_KeyState.Down = state;
    }

    if (state == KEY_STATE_DOWN) return;

    if (s_KeyMap.Enter == scancode
        || s_KeyMap.Debug == scancode)
    {
        SDL_Event user;
        user.type = SDL_USEREVENT;
        user.user.code = s_KeyMap.Enter == scancode ? ENTER : DEBUG;

        SDL_PushEvent(&user);
    }
}
