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
#include "Renderer.h"
#include "Screens.h"

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 1280

static void QuitGame(void);
static void UpdateGame();
static void ChangeToScreen(GameScreen screen);
static void HandlePause(void);
static void HandleEnter(void);

GameScreen currentScreen = UNKNOWN;
static GameScreen previousScreen = UNKNOWN;
static const Vector2D fpsPosition = { 10, 10 };

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char* argv[])
{
    GAM_InitializeGameSystems("Brick Breaker", WINDOW_WIDTH, WINDOW_HEIGHT);
    ChangeToScreen(TITLE);

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
                if (event.user.code == PAUSE)
                {
                    HandlePause();
                }
                else if (event.user.code == ENTER)
                {
                    HandleEnter();
                }

                continue;
            }

            KBD_HandleEvent(&event);
        }

        UpdateGame();
        GAM_EndFrame();
    }

    QuitGame();

    return 0;
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

   GAM_ShutdownGameSystems();
}

void UpdateGame()
{
    const float deltaTime = GAM_GetDeltaSeconds();

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

    REN_BeginDrawing();

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

    GAM_DrawFPS(fpsPosition);
    REN_FinishDrawing();
}

void ChangeToScreen(const GameScreen screen)
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

void HandlePause(void)
{
    if (GAM_GetIsPaused()) return;

    GAM_PauseGame();
    ChangeToScreen(OPTIONS);
}

void HandleEnter(void)
{
    switch (currentScreen)
    {
        case UNKNOWN:
            break;
        case TITLE:
            TitleEnterKeyPressed();
            break;
        case OPTIONS:
            OptionsEnterKeyPressed();
            break;
        case GAMEPLAY:
            GameplayEnterKeyPressed();
            break;
        case ENDING:
            EndingEnterKeyPressed();
            break;
    }
}
