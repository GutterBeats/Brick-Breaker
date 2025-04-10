//
// Created by Anthony Lesch on 07/15/24
//
// main.c
//
// ReSharper disable CppParameterNeverUsed

#include <SDL_main.h>

#include "EventSystem.h"
#include "Game.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

int main(int argc, char* argv[])
{
    static const Vector2D fpsPosition = { 5, 5 };

    GAM_InitializeGameSystems("Brick Breaker", WINDOW_WIDTH, WINDOW_HEIGHT);

    GAM_TransitionToScene(&TitleScene);

    while (GAM_GetIsGameRunning())
    {
        GAM_StartFrame();
        EVT_PollEvents();
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
