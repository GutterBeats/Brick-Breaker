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

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

static void QuitGame(void);
static void UpdateGame(void);
static void ChangeToScreen(GameScreen screen);
static void HandleEnter(void);
static void HandlePause(void);
static void ToggleDebug(void);

static GameScreen currentScreen = UNKNOWN;
static GameScreen previousScreen = UNKNOWN;
static const Vector2D fpsPosition = { 5, 5 };

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
                switch (event.user.code)
                {
                    case ENTER: HandleEnter(); break;
                    case DEBUG: ToggleDebug(); break;
                    case PAUSE: HandlePause(); break;
                    default: break;
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
        case UNKNOWN: break;
        case TITLE: UnloadTitleScreen(); break;
        case MENU: UnloadMenuScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case GAMEOVER: UnloadGameOverScreen(); break;
    }

   GAM_ShutdownGameSystems();
}

void UpdateGame(void)
{
    const float deltaTime = GAM_GetDeltaSeconds();

    switch (currentScreen)
    {
        case UNKNOWN: break;
        case TITLE:
        {
            UpdateTitleScreen(deltaTime);

            if (FinishTitleScreen())
            {
                ChangeToScreen(GAMEPLAY);
            }

            break;
        }
        case MENU:
        {
            UpdateMenuScreen(deltaTime);
            ChangeToScreen(FinishMenuScreen());

            break;
        }
        case OPTIONS:
        {
            UpdateOptionsScreen(deltaTime);

            if (FinishOptionsScreen())
            {
                ChangeToScreen(MENU);
            }

            break;
        }
        case GAMEPLAY:
        {
            UpdateGameplayScreen(deltaTime);

            if (FinishGameplayScreen())
            {
                ChangeToScreen(GAMEOVER);
            }

            break;
        }
        case GAMEOVER:
        {
            UpdateGameOverScreen(deltaTime);

            if (FinishGameOverScreen())
            {
                ChangeToScreen(GAMEPLAY);
            }
            break;
        }
    }

    REN_BeginDrawing();

    switch (currentScreen)
    {
        case UNKNOWN: break;
        case MENU: DrawMenuScreen(); break;
        case OPTIONS: DrawOptionsScreen(); break;
        case GAMEPLAY: DrawGameplayScreen(); break;
        case TITLE: DrawTitleScreen(); break;
        case GAMEOVER: DrawGameOverScreen(); break;
    }

    if (GAM_GetShowDebug())
    {
        GAM_DrawFPS(fpsPosition);
    }

    REN_FinishDrawing();
}

void ChangeToScreen(const GameScreen screen)
{
    if (currentScreen == screen) return;

    // Unload current screen
    switch (currentScreen)
    {
        case UNKNOWN:
        case MENU: UnloadMenuScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case GAMEOVER: UnloadGameOverScreen(); break;
    }

    // Init next screen
    switch (screen)
    {
        case UNKNOWN:
        case TITLE: InitTitleScreen(); break;
        case MENU: InitMenuScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case GAMEOVER: InitGameOverScreen(); break;
    }

    previousScreen = currentScreen;
    currentScreen = screen;
}

void HandleEnter(void)
{
    switch (currentScreen)
    {
        case UNKNOWN: break;
        case TITLE: TitleEnterKeyPressed(); break;
        case MENU: MenuEnterKeyPressed(); break;
        case OPTIONS: OptionsEnterKeyPressed(); break;
        case GAMEPLAY: GameplayEnterKeyPressed(); break;
        case GAMEOVER: GameOverEnterKeyPressed(); break;
    }
}

void ToggleDebug(void)
{
    GAM_SetShowDebug(
        !GAM_GetShowDebug());
}

void HandlePause(void)
{
    if (currentScreen != GAMEPLAY) return;

    GAM_SetIsPaused(
        !GAM_GetIsPaused());
}
