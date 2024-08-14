//
// Created by Anthony Lesch on 7/15/24.
//

#include "Keyboard.h"

#include "Utils.h"

static KeyMap s_KeyMap;
static KeyState s_KeyState;

static KeyArray* CreateKeys(int count, ...);
static void FreeKeyArray(KeyArray* keys);
static void HandleRealtimeKeys(SDL_Scancode scancode, enum KBD_KEY_STATE state);
static bool ContainsKey(KeyArray* keys, SDL_Scancode scancode);

void KBD_InitializeKeymap()
{
    s_KeyMap.Left = CreateKeys(2, SDL_SCANCODE_A, SDL_SCANCODE_LEFT);
    s_KeyMap.Right = CreateKeys(2, SDL_SCANCODE_D, SDL_SCANCODE_RIGHT);
    s_KeyMap.Up = CreateKeys(2, SDL_SCANCODE_W, SDL_SCANCODE_UP);
    s_KeyMap.Down = CreateKeys(2, SDL_SCANCODE_S, SDL_SCANCODE_DOWN);
    s_KeyMap.Enter = SDL_SCANCODE_SPACE;
    s_KeyMap.Debug = SDL_SCANCODE_1;

    s_KeyState.Left = KEY_STATE_UP;
    s_KeyState.Right = KEY_STATE_UP;
    s_KeyState.Up = KEY_STATE_UP;
    s_KeyState.Down = KEY_STATE_UP;
}

void KBD_DestroyKeymap(void)
{
    FreeKeyArray(s_KeyMap.Left);
    FreeKeyArray(s_KeyMap.Right);
    FreeKeyArray(s_KeyMap.Up);
    FreeKeyArray(s_KeyMap.Down);
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

static KeyArray* CreateKeys(const int count, ...)
{
    KeyArray* keys = malloc(sizeof(KeyArray));
    if (keys == NULL)
    {
        BB_LOG("Unable to allocate memory for key array.");
        return NULL;
    }

    keys->Count = count;
    keys->Keys = malloc(count * sizeof(SDL_Scancode));

    if (keys->Keys == NULL)
    {
        free(keys);
        BB_LOG("Unable to allocate memory for keys.");

        return NULL;
    }

    va_list codes;
    va_start(codes, count);

    for (int i = 0; i < count; ++i)
    {
        keys->Keys[i] = va_arg(codes, SDL_Scancode);
    }

    va_end(codes);

    return keys;
}

static void FreeKeyArray(KeyArray* keys)
{
    free(keys->Keys);
    free(keys);
}

static void HandleRealtimeKeys(const SDL_Scancode scancode, const enum KBD_KEY_STATE state)
{
    if (ContainsKey(s_KeyMap.Left, scancode))
    {
        s_KeyState.Left = state;
    }
    else if (ContainsKey(s_KeyMap.Right, scancode))
    {
        s_KeyState.Right = state;
    }

    if (ContainsKey(s_KeyMap.Up, scancode))
    {
        s_KeyState.Up = state;
    }
    else if (ContainsKey(s_KeyMap.Down, scancode))
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

static bool ContainsKey(KeyArray* keys, SDL_Scancode scancode)
{
    for (int i = 0; i < keys->Count; ++i)
    {
        if (keys->Keys[i] == scancode)
        {
            return true;
        }
    }

    return false;
}
