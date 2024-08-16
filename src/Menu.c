//
// Created by Anthony Lesch on 8/14/24.
//

#include "ButtonContainer.h"
#include "Renderer.h"
#include "Resources.h"
#include "Types.h"
#include "Utils.h"

#define BUTTON_COUNT 3
#define BUTTON_CONTAINER_X 205
#define BUTTON_CONTAINER_Y 351

//----------------------------------------------------------------------------------
// Menu Screen Variables
//----------------------------------------------------------------------------------
static Texture* background;
static ButtonContainer* buttonContainer;
static GameScreen nextScreen;

//----------------------------------------------------------------------------------
// Menu Screen Helper Functions
//----------------------------------------------------------------------------------
static void PlayButtonClicked(void);
static void OptionsButtonClicked(void);
static void ExitButtonClicked(void);

//----------------------------------------------------------------------------------
// Menu Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitMenuScreen(void)
{
    nextScreen = MENU;
    background = REN_LoadTexture(MENU_BACKGROUND_IMAGE);
    buttonContainer = BC_CreateContainer(
        UTL_MakeVectorF2D(BUTTON_CONTAINER_X, BUTTON_CONTAINER_Y), BUTTON_COUNT);

    BC_CreateButton(buttonContainer, "PLAY", &PlayButtonClicked);
    BC_CreateButton(buttonContainer, "OPTIONS", &OptionsButtonClicked);
    BC_CreateButton(buttonContainer, "EXIT", &ExitButtonClicked);
}

void UpdateMenuScreen(float deltaTime)
{
}

void DrawMenuScreen(void)
{
    REN_DrawTexture(background, UTL_GetZeroVector());
    BC_DrawButtonContainer(buttonContainer);
}

void UnloadMenuScreen(void)
{
    REN_FreeTexture(background);
    BC_DestroyContainer(buttonContainer);
}

GameScreen FinishMenuScreen(void)
{
    return nextScreen;
}

void MenuEnterKeyPressed(void)
{
    BC_ClickSelectedItem(buttonContainer);
}

void MenuUpKeyPressed(void)
{
    BC_ChangeSelectedIndex(buttonContainer, UP);
}

void MenuDownKeyPressed(void)
{
    BC_ChangeSelectedIndex(buttonContainer, DOWN);
}

static void PlayButtonClicked(void)
{
    nextScreen = GAMEPLAY;
}

static void OptionsButtonClicked(void)
{
    nextScreen = OPTIONS;
}

static void ExitButtonClicked(void)
{
    SDL_Event quit = { SDL_QUIT };

    SDL_PushEvent(&quit);
}
