//
// Created by Anthony Lesch on 7/20/24.
//

#include "Game.h"

#include "Audio.h"
#include "Keyboard.h"
#include "Renderer.h"
#include "Text.h"
#include "Utils.h"

#define MAX_FRAME_AVG 120

#if __APPLE__
#define MENU_BAR_HEIGHT 80
#else
#define MENU_BAR_HEIGHT 0
#endif

static Game game;
static SDL_Window* window;
static float lastFrame = 0;
static float currentFrame = 0;
static int scaledFrameCount = 0;

void InitializeGameSystems(const char* title, int desiredScreenWidth, int desiredScreenHeight)
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Log("Could not initialize SDL!: %s", SDL_GetError());
        return;
    }

    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) == 0)
    {
        const float desiredRatio = (float)desiredScreenWidth / (float)desiredScreenHeight;

        const int actualWidth = dm.w;
        const int actualHeight = dm.h - MENU_BAR_HEIGHT;

        if (desiredScreenWidth > actualWidth)
        {
            desiredScreenWidth = actualWidth;
            desiredScreenHeight = actualHeight * desiredRatio;
        }

        if (desiredScreenHeight > actualHeight)
        {
            desiredScreenWidth = actualWidth * desiredRatio;
            desiredScreenHeight = actualHeight;
        }
    }
    else
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
    }

    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              desiredScreenWidth, desiredScreenHeight,
                              SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        SDL_Log("Unable to create a window!: %s", SDL_GetError());
        return;
    }

    if (!InitializeRenderer(window))
    {
        return;
    }

    InitializeText();
    InitializeAudioSystem();

    KBD_InitializeKeymap();
    UTL_SetRandomSeed();

    lastFrame = SDL_GetTicks64();

    game = (Game){
        .ScreenWidth = desiredScreenWidth,
        .ScreenHeight = desiredScreenHeight,
        .TimeScale = 1.f,
        .DeltaSeconds = 0.f,
        .FPS = 0.f,
        .IsRunning = true,
        .IsPaused = false,
    };
}

void ShutdownGameSystems(void)
{
    DestroyRenderer();

    DestroyTextSystem();
    DestroyAudioSystem();

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GetScreenDimensions(int* width, int* height)
{
    if (width != NULL)
    {
        *width = game.ScreenWidth;
    }

    if (height != NULL)
    {
        *height = game.ScreenHeight;
    }
}

bool GetIsGameRunning(void)
{
    return game.IsRunning;
}

void SetIsGameRunning(const bool running)
{
    game.IsRunning = running;
}

bool GetIsPaused(void)
{
    return game.IsPaused;
}

void PauseGame(void)
{
    game.IsPaused = true;
}

void UnpauseGame(void)
{
    game.IsPaused = false;
}

void StartFrame()
{
    currentFrame = SDL_GetTicks64();
    game.DeltaSeconds = (currentFrame - lastFrame) / 1000.f;

    if (scaledFrameCount > 0)
    {
        SDL_Log("Scaled for %d frames", scaledFrameCount);

        if (--scaledFrameCount == 0)
        {
            SDL_Log("Resetting scaled frame time.");
            SetTimeScaleForFrames(1.f, 0);
        }
    }

    static int frameCount = 0;
    static float total = 0;
    static float time = 0;

    if (frameCount++ < MAX_FRAME_AVG && time <= 1.f)
    {
        total += game.DeltaSeconds;
        time += game.DeltaSeconds;

        return;
    }

    const float average = total / (float)frameCount;

    frameCount = 0;
    time = 0.f;
    total = 0.f;

    game.FPS = 1000.f / (average * 1000.f);
    SDL_Log("FPS: %6.2f", game.FPS);
}

void EndFrame()
{
    lastFrame = currentFrame;
}

void SetTimeScale(const float scale)
{
    game.TimeScale = UTL_FClamp(0.1f, 3.f, scale);
}

void SetTimeScaleForFrames(const float scale, const int frameCount)
{
    SDL_Log("Setting time scale to %.2f for %d frames.", scale, frameCount);

    scaledFrameCount = frameCount;
    SetTimeScale(scale);
}

float GetDeltaSeconds()
{
    return game.DeltaSeconds * game.TimeScale;
}

float GetFPS()
{
    return game.FPS;
}

void DrawFPS(const int x, const int y)
{
    char buffer[10];
    sprintf(buffer, "FPS: %d", (int)game.FPS);

    DrawText(buffer, x, y);
}
