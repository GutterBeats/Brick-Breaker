//
// Created by Anthony Lesch on 7/20/24.
//

#include "Game.h"
#include "Audio.h"
#include "Keyboard.h"
#include "Renderer.h"
#include "Text.h"
#include "Utils.h"

#if __APPLE__
#define MENU_BAR_HEIGHT 80
#else
#define MENU_BAR_HEIGHT 0
#endif

static void CalculateFPS(void);

#define MAX_FPS 60
#define TICKS_PER_FRAME 1000 / MAX_FPS

static Game game;
static SDL_Window* window;
static float lastFrame = 0;
static float currentFrame = 0;
static float scaledFrameSeconds = 0.f;

void GAM_InitializeGameSystems(const char* title, int desiredScreenWidth, int desiredScreenHeight)
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        BB_LOG("Could not initialize SDL!: %s", SDL_GetError());
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
        BB_LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
    }

    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              desiredScreenWidth, desiredScreenHeight,
                              SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        BB_LOG("Unable to create a window!: %s", SDL_GetError());
        return;
    }

    if (!InitializeRenderer(window))
    {
        return;
    }

    TXT_InitializeText();
    AUD_InitializeAudioSystem();

    KBD_InitializeKeymap();
    UTL_SetRandomSeed();

    lastFrame = SDL_GetTicks64();

    game = (Game){
        .ScreenWidth = desiredScreenWidth,
        .ScreenHeight = desiredScreenHeight,
        .Score = 0,
        .TimeScale = 1.f,
        .DeltaSeconds = 0.f,
        .FPS = 0.f,
        .GameWon = false,
        .IsRunning = true,
        .ShowDebug = false,
    };
}

void GAM_ShutdownGameSystems(void)
{
    REN_DestroyRenderer();
    KBD_DestroyKeymap();
    TXT_DestroyTextSystem();
    AUD_DestroyAudioSystem();

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GAM_GetScreenDimensions(int* width, int* height)
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

void GAM_UpdateScore(const int points)
{
    game.Score += points;
}

int GAM_GetScore(void)
{
    return game.Score;
}

bool GAM_GetGameWon(void)
{
    return game.GameWon;
}

void GAM_SetGameWon(const bool value)
{
    game.GameWon = value;
}

bool GAM_GetIsGameRunning(void)
{
    return game.IsRunning;
}

void GAM_SetIsGameRunning(const bool running)
{
    game.IsRunning = running;
}

void GAM_StartFrame(void)
{
    currentFrame = SDL_GetTicks64();
    game.DeltaSeconds = (currentFrame - lastFrame) / 1000.f;

    if (scaledFrameSeconds > 0)
    {
        BB_LOG("Scaled for %.2f seconds", scaledFrameSeconds);

        scaledFrameSeconds -= game.DeltaSeconds;
        if (scaledFrameSeconds <= 0)
        {
            BB_LOG("Resetting scaled frame time.");
            GAM_SetTimeScaleForSeconds(1.f, 0);
        }
    }

    CalculateFPS();

    const int frameTicks = SDL_GetTicks64() - currentFrame;
    if (frameTicks < TICKS_PER_FRAME)
    {
        //Wait remaining time
        SDL_Delay(TICKS_PER_FRAME - frameTicks);
    }
}

void GAM_EndFrame(void)
{
    lastFrame = currentFrame;
}

void GAM_SetTimeScale(const float scale)
{
    game.TimeScale = UTL_FClamp(0.1f, 5.f, scale);
}

void GAM_SetTimeScaleForSeconds(const float scale, const float seconds)
{
    BB_LOG("Setting time scale to %.2f for %.2f frames.", scale, seconds);

    scaledFrameSeconds = seconds;
    GAM_SetTimeScale(scale);
}

float GAM_GetDeltaSeconds(void)
{
    return game.DeltaSeconds * game.TimeScale;
}

float GAM_GetFPS(void)
{
    return game.FPS;
}

void GAM_DrawFPS(const Vector2D position)
{
    char buffer[10];
    sprintf(buffer, "FPS: %d", (int)game.FPS);

    TXT_DrawText(buffer, position);
}

bool GAM_GetShowDebug(void)
{
    return game.ShowDebug;
}

void GAM_SetShowDebug(const bool showDebug)
{
    game.ShowDebug = showDebug;
}

static void CalculateFPS(void)
{
    static int frameCount = 0;
    static float total = 0;

    if (total <= 1.1f)
    {
        total += game.DeltaSeconds;
        ++frameCount;

        return;
    }

    const float average = total / (float)frameCount;

    frameCount = 0;
    total = 0.f;

    game.FPS = 1000.f / (average * 1000.f);

    BB_LOG("FPS: %6.2f", game.FPS);
}
