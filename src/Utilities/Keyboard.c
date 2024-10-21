//
// Created by Anthony Lesch on 7/15/24.
//

#include "Keyboard.h"
#include "Utilities.h"

enum KBD_KEY_STATE
{
    KEY_STATE_UP,
    KEY_STATE_DOWN
};

typedef struct key_state
{
    enum KBD_KEY_STATE Left;
    enum KBD_KEY_STATE Right;
} KeyState;

static KeyMap s_KeyMap;
static KeyState s_KeyState;

static KeyArray* CreateKeys(int count, ...);
static void FreeKeyArray(KeyArray* keys);
static void HandleRealtimeKeys(SDL_Keycode keycode, enum KBD_KEY_STATE state);
static bool ContainsKey(const KeyArray* keys, SDL_Keycode keycode);

void KBD_InitializeKeymap()
{
    s_KeyMap.Left = CreateKeys(2, SDLK_a, SDLK_LEFT);
    s_KeyMap.Right = CreateKeys(2, SDLK_d, SDLK_RIGHT);
    s_KeyMap.Up = CreateKeys(2, SDLK_w, SDLK_UP);
    s_KeyMap.Down = CreateKeys(2, SDLK_s, SDLK_DOWN);
    s_KeyMap.Enter = CreateKeys(3, SDLK_SPACE, SDLK_KP_ENTER, SDLK_RETURN);
    s_KeyMap.Debug = SDLK_1;
    s_KeyMap.Pause = SDLK_ESCAPE;

    s_KeyState.Left = KEY_STATE_UP;
    s_KeyState.Right = KEY_STATE_UP;
}

void KBD_DestroyKeymap(void)
{
    FreeKeyArray(s_KeyMap.Left);
    FreeKeyArray(s_KeyMap.Right);
    FreeKeyArray(s_KeyMap.Up);
    FreeKeyArray(s_KeyMap.Down);
    FreeKeyArray(s_KeyMap.Enter);
}

void KBD_HandleEvent(const SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN:
            HandleRealtimeKeys(event->key.keysym.sym, KEY_STATE_DOWN);
            break;
        case SDL_KEYUP:
            HandleRealtimeKeys(event->key.keysym.sym, KEY_STATE_UP);
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
    keys->Keys = malloc(count * sizeof(SDL_Keycode));

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
        keys->Keys[i] = va_arg(codes, SDL_Keycode);
    }

    va_end(codes);

    return keys;
}

static void FreeKeyArray(KeyArray* keys)
{
    free(keys->Keys);
    free(keys);
}

static void HandleRealtimeKeys(const SDL_Keycode keycode, const enum KBD_KEY_STATE state)
{
    if (ContainsKey(s_KeyMap.Left, keycode))
    {
        s_KeyState.Left = state;
    }
    else if (ContainsKey(s_KeyMap.Right, keycode))
    {
        s_KeyState.Right = state;
    }

    if (state == KEY_STATE_DOWN) return;

    if (ContainsKey(s_KeyMap.Up, keycode))
    {
        SDL_Event upKeyPressed = { SDL_USEREVENT };
        upKeyPressed.user.code = UP_KEY_PRESSED;

        SDL_PushEvent(&upKeyPressed);
        return;
    }

    if (ContainsKey(s_KeyMap.Down, keycode))
    {
        SDL_Event upKeyPressed = { SDL_USEREVENT };
        upKeyPressed.user.code = DOWN_KEY_PRESSED;

        SDL_PushEvent(&upKeyPressed);
        return;
    }

    if (ContainsKey(s_KeyMap.Enter, keycode)
        || s_KeyMap.Debug == keycode
        || s_KeyMap.Pause == keycode)
    {
        SDL_Event user;
        user.type = SDL_USEREVENT;
        user.user.code = ContainsKey(s_KeyMap.Enter, keycode) ? ENTER
                                                              : s_KeyMap.Debug == keycode ? DEBUG : PAUSE;

        SDL_PushEvent(&user);
    }
}

static bool ContainsKey(const KeyArray* keys, const SDL_Keycode keycode)
{
    for (int i = 0; i < keys->Count; ++i)
    {
        if (keys->Keys[i] == keycode)
        {
            return true;
        }
    }

    return false;
}
