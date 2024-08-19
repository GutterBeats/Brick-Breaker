//
// Created by Anthony Lesch on 07/15/24
//
// main.c
//
// ReSharper disable CppParameterNeverUsed

#include <SDL.h>
#include <SDL_main.h>

#include "Game.h"
#include "Keyboard.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

static const Vector2D fpsPosition = { 5, 5 };

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char* argv[])
{
    GAM_InitializeGameSystems("Brick Breaker", WINDOW_WIDTH, WINDOW_HEIGHT);
    GAM_TransitionToScene(&TitleScene);

    while (GAM_GetIsGameRunning())
    {
        GAM_StartFrame();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                GAM_SetIsGameRunning(false);
                break;
            }

            if (event.type == SDL_USEREVENT)
            {
                //HandleUserEvent(event.user.code);
                continue;
            }

            KBD_HandleEvent(&event);
        }

        GAM_UpdateCurrentScene();

        if (GAM_GetShowDebug())
        {
            GAM_DrawFPS(fpsPosition);
        }

        GAM_EndFrame();
    }

    GAM_ShutdownGameSystems();

    return 0;
}
