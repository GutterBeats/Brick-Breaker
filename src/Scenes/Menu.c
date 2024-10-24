//
// Created by Anthony Lesch on 8/14/24.
//

#include "Audio.h"
#include "ButtonContainer.h"
#include "EventSystem.h"
#include "Game.h"
#include "Renderer.h"
#include "Resources.h"
#include "Types.h"
#include "Utilities.h"

#define BUTTON_COUNT 2
#define BUTTON_CONTAINER_X 205
#define BUTTON_CONTAINER_Y 351

//----------------------------------------------------------------------------------
// Menu Screen Variables
//----------------------------------------------------------------------------------
static Texture* background;
static ButtonContainer* buttonContainer;
static i8 moveUpSfx;
static i8 moveDownSfx;
static i8 itemSelectedSfx;

//----------------------------------------------------------------------------------
// Menu Screen Helper Functions
//----------------------------------------------------------------------------------
static void EnterKeyPressed(void);
static void UpKeyPressed(void);
static void DownKeyPressed(void);
static void PlayButtonClicked(void);
static void ExitButtonClicked(void);

//----------------------------------------------------------------------------------
// Menu Screen Functions Declaration
//----------------------------------------------------------------------------------
static void Initialize(void);
static void Draw(void);
static void Destroy(void);

//----------------------------------------------------------------------------------
// Menu Scene Extern
//----------------------------------------------------------------------------------
Scene MenuScene = {
    .Name = "MENU SCENE",
    .Initialize = Initialize,
    .Draw = Draw,
    .Destroy = Destroy
};

void Initialize(void)
{
    background = REN_LoadTexture(MENU_BACKGROUND_IMAGE);
    buttonContainer = BC_CreateContainer(
        UTL_MakeVectorF2D(BUTTON_CONTAINER_X, BUTTON_CONTAINER_Y), BUTTON_COUNT);

    BC_CreateButton(buttonContainer, "PLAY", &PlayButtonClicked);
    BC_CreateButton(buttonContainer, "EXIT", &ExitButtonClicked);

    moveUpSfx = AUD_LoadSoundEffect(MOVE_UP_SFX);
    moveDownSfx = AUD_LoadSoundEffect(MOVE_DOWN_SFX);
    itemSelectedSfx = AUD_LoadSoundEffect(ITEM_SELECTED_SFX);

    AUD_PlayMusic(MENU_MUSIC);

    EVT_BindUserEvent(ENTER, EnterKeyPressed);
    EVT_BindUserEvent(UP_KEY_PRESSED, UpKeyPressed);
    EVT_BindUserEvent(DOWN_KEY_PRESSED, DownKeyPressed);
}

void Draw(void)
{
    REN_DrawTexture(background, UTL_GetZeroVector());
    BC_DrawButtonContainer(buttonContainer);
}

void Destroy(void)
{
    REN_FreeTexture(background);
    BC_DestroyContainer(buttonContainer);

    AUD_UnloadSoundEffect(moveUpSfx);
    AUD_UnloadSoundEffect(moveDownSfx);
    AUD_UnloadSoundEffect(itemSelectedSfx);
}

void EnterKeyPressed(void)
{
    AUD_PlaySoundEffect(itemSelectedSfx);

    // HACK: Item Selected sound effect gets unloaded before it finishes playing.
    SDL_Delay(500);

    BC_ClickSelectedItem(buttonContainer);
}

void UpKeyPressed(void)
{
    AUD_PlaySoundEffect(moveUpSfx);
    BC_ChangeSelectedIndex(buttonContainer, UP);
}

void DownKeyPressed(void)
{
    AUD_PlaySoundEffect(moveDownSfx);
    BC_ChangeSelectedIndex(buttonContainer, DOWN);
}

static void PlayButtonClicked(void)
{
    GAM_TransitionToScene(&GameplayScene);
}

static void ExitButtonClicked(void)
{
    SDL_Event quit = { SDL_QUIT };

    SDL_PushEvent(&quit);
}
