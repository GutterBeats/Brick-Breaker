//
// Created by Anthony Lesch on 07/15/24
//
// main.c
//

#include <SDL.h>

#include "Game.h"
#include "Keyboard.h"
#include "Screens.h"
#include "Renderer.h"

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 1280

static void QuitGame(void);
static void UpdateGame();
static void ChangeToScreen(GameScreen screen);
static void HandlePause(void);
static void HandleEnter(void);

GameScreen currentScreen = UNKNOWN;
static GameScreen previousScreen = UNKNOWN;

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char* argv[])
{
    InitializeGameSystems("Brick Breaker", WINDOW_WIDTH, WINDOW_HEIGHT);
    ChangeToScreen(GAMEPLAY);

    while (GetIsGameRunning())
    {
        StartFrame();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SetIsGameRunning(false);
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
        EndFrame();
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

   ShutdownGameSystems();
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
void UpdateGame()
{
    const float deltaTime = GetDeltaSeconds();

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

    DrawFPS(10, 10);
    FinishDrawing();
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
    if (GetIsPaused()) return;

    PauseGame();
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
