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
    s_KeyMap.Pause = SDL_SCANCODE_ESCAPE;

    s_KeyState.Left = KEY_STATE_UP;
    s_KeyState.Right = KEY_STATE_UP;
}

void KBD_HandleEvent(const SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN:
            HandleRealtimeKeys(event->key.keysym.scancode, KEY_STATE_DOWN);
            break;
        case SDL_KEYUP:
        {
            HandleRealtimeKeys(event->key.keysym.scancode, KEY_STATE_UP);

            if (event->key.keysym.scancode == s_KeyMap.Pause)
            {
                SDL_Event pause;
                pause.type = SDL_USEREVENT;

                SDL_PushEvent(&pause);

                return;
            }
        }
            break;
        default:
            break;
    }
}

SDL_bool KBD_IsLeftKeyDown()
{
    return s_KeyState.Left == KEY_STATE_DOWN;
}

SDL_bool KBD_IsRightKeyDown()
{
    return s_KeyState.Right == KEY_STATE_DOWN;
}

static void HandleRealtimeKeys(SDL_Scancode scancode, enum KBD_KEY_STATE state)
{
    if (s_KeyMap.Left == scancode)
    {
        s_KeyState.Left = state;
    }
    else if (s_KeyMap.Right == scancode)
    {
        s_KeyState.Right = state;
    }
}
