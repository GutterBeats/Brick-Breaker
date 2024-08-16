//
// Created by Anthony Lesch on 8/14/24.
//

#include "Audio.h"
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
static i8 moveUpSfx;
static i8 moveDownSfx;
static i8 itemSelectedSfx;

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

    moveUpSfx = AUD_LoadSoundEffect(MOVE_UP_SFX);
    moveDownSfx = AUD_LoadSoundEffect(MOVE_DOWN_SFX);
    itemSelectedSfx = AUD_LoadSoundEffect(ITEM_SELECTED_SFX);

    AUD_PlayMusic(MENU_MUSIC);
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

    AUD_UnloadSoundEffect(moveUpSfx);
    AUD_UnloadSoundEffect(moveDownSfx);
    AUD_UnloadSoundEffect(itemSelectedSfx);
}

GameScreen FinishMenuScreen(void)
{
    return nextScreen;
}

void MenuEnterKeyPressed(void)
{
    AUD_PlaySoundEffect(itemSelectedSfx);
    BC_ClickSelectedItem(buttonContainer);
}

void MenuUpKeyPressed(void)
{
    AUD_PlaySoundEffect(moveUpSfx);
    BC_ChangeSelectedIndex(buttonContainer, UP);
}

void MenuDownKeyPressed(void)
{
    AUD_PlaySoundEffect(moveDownSfx);
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
