//
// Created by Anthony Lesch on 07/15/24
//
// main.c
//

#include <SDL.h>

#include "Keyboard.h"
#include "Screens.h"
#include "Renderer.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static void InitializeSystems(void);
static void QuitGame(void);
static void PauseGame(void);
static void UpdateGame(float deltaTime);
static void ChangeToScreen(GameScreen screen);

GameScreen currentScreen = GAMEPLAY;
static enum GameScreen previousScreen = UNKNOWN;

static SDL_Window* window;
static bool isRunning = false;
static bool isPaused = false;

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char* argv[])
{
    InitializeSystems();
    ChangeToScreen(GAMEPLAY);

    u64 lastFrame = SDL_GetTicks64();

    while (isRunning)
    {
        u64 current = SDL_GetTicks64();
        float deltaTime = ((float)current - (float)lastFrame) / 1000.f;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = SDL_FALSE;
                break;
            }

            if (event.type == SDL_USEREVENT)
            {
                PauseGame();
                continue;
            }

            KBD_HandleEvent(&event);
        }

        UpdateGame(deltaTime);

        lastFrame = current;
    }

    QuitGame();

    return 0;
}

void InitializeSystems(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Log("Could not initialize SDL!: %s", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow("Brick Breaker",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_METAL);
    if (window == NULL)
    {
        SDL_Log("Unable to create a window!: %s", SDL_GetError());
        return;
    }

    if (!InitializeRenderer(window))
    {
        return;
    }

    KBD_InitializeKeymap();

    isRunning = true;
}

void QuitGame(void)
{
    switch (currentScreen)
    {
        case UNKNOWN:
            break;
        case TITLE:
            UnloadTitleScreen();
            break;
        case OPTIONS:
            UnloadOptionsScreen();
            break;
        case GAMEPLAY:
            UnloadGameplayScreen();
            break;
        case ENDING:
            UnloadEndingScreen();
            break;
    }

    DestroyRenderer();

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void PauseGame(void)
{
    if (isPaused) return;

    isPaused = true;
    ChangeToScreen(OPTIONS);
}

/*
 * Main Gameplay loop.
 *
 * Update Loop -
 * Update Player Position (Check for Collision with walls, ball) ->
 * Update Ball Position (Check for Collision with player, walls, bricks, floor) ->
 * On Collision with Brick, take health away and optionally update texture that represents it.
 *
 * Draw Loop -
 * Draw Player -> Draw Ball -> Draw Bricks
 *
 * (Optional)
 * Add score
 * Add particles
 * Add Game Over Screen
 * Add Main Menu
 * Add Options
 * */
void UpdateGame(float deltaTime)
{
    switch (currentScreen)
    {
        case GAMEPLAY:
        {
            UpdateGameplayScreen(deltaTime);

            if (FinishGameplayScreen())
            {
                ChangeToScreen(ENDING);
            }

            break;
        }
        case UNKNOWN:
            break;
        case TITLE:
            UpdateTitleScreen(deltaTime);

            if (FinishTitleScreen())
            {
                ChangeToScreen(GAMEPLAY);
            }
            break;
        case OPTIONS:
            UpdateOptionsScreen(deltaTime);

            if (FinishOptionsScreen())
            {
                ChangeToScreen(previousScreen);
            }
            break;
        case ENDING:
            UpdateEndingScreen(deltaTime);

            if (FinishEndingScreen())
            {
                ChangeToScreen(TITLE);
            }
            break;
    }

    BeginDrawing();

    switch (currentScreen)
    {
        case GAMEPLAY:
            DrawGameplayScreen();
            break;
        case TITLE:
            DrawTitleScreen();
            break;
        case OPTIONS:
            DrawOptionsScreen();
            break;
        case ENDING:
            DrawEndingScreen();
            break;
        default:
            break;
    }

    FinishDrawing();
}

void ChangeToScreen(GameScreen screen)
{
    if (currentScreen == screen) return;

    // Unload current screen
    switch (currentScreen)
    {
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen)
    {
        case UNKNOWN:
        case TITLE: InitTitleScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        default: break;
    }

    previousScreen = currentScreen;
    currentScreen = screen;
}
