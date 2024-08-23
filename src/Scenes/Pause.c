//
// Created by Anthony Lesch on 8/19/24.
//

#include "Audio.h"
#include "ButtonContainer.h"
#include "EventSystem.h"
#include "Game.h"
#include "Renderer.h"
#include "Resources.h"
#include "Types.h"
#include "Utils.h"

#define BUTTON_CONTAINER_X 205
#define BUTTON_CONTAINER_Y_OFFSET 25

//----------------------------------------------------------------------------------
// Pause Screen Variables
//----------------------------------------------------------------------------------
static Texture* pause = NULL;
static ButtonContainer* buttonContainer = NULL;
static VectorF2D pauseLocation;
static i8 currentMusicVolume;

//----------------------------------------------------------------------------------
// Pause Helper Functions
//----------------------------------------------------------------------------------
static void EnterKeyPressed(void);
static void PauseKeyPressed(void);
static void ResumeClicked(void);
static void ReturnToMenuClicked(void);
static void UpKeyClicked(void);
static void DownKeyClicked(void);

//----------------------------------------------------------------------------------
// Pause Scene Functions
//----------------------------------------------------------------------------------
static void Initialize(void);
static void Draw(void);
static void Destroy(void);

Scene PauseScene = {
    .Name = "PAUSE SCENE",
    .Initialize = Initialize,
    .Draw = Draw,
    .Destroy = Destroy
};

static void Initialize(void)
{
    pause = REN_LoadTexture(PAUSE_TEXTURE);
    ASSERT_NOTNULL(pause, "Pause Background")

    int width, height;
    GAM_GetScreenDimensions(&width, &height);

    pauseLocation = UTL_MakeVectorF2D(
        width / 2.f - pause->Width / 2.f,
        height / 3.f - pause->Height / 2.f);

    EVT_BindUserEvent(ENTER, EnterKeyPressed);
    EVT_BindUserEvent(PAUSE, PauseKeyPressed);
    EVT_BindUserEvent(UP_KEY_PRESSED, UpKeyClicked);
    EVT_BindUserEvent(DOWN_KEY_PRESSED, DownKeyClicked);

    buttonContainer = BC_CreateContainer(
        UTL_MakeVectorF2D(BUTTON_CONTAINER_X, pauseLocation.Y + pause->Height + BUTTON_CONTAINER_Y_OFFSET), 2);

    ASSERT_NOTNULL(buttonContainer, "Pause Button Container")

    BC_CreateButton(buttonContainer, "Resume", ResumeClicked);
    BC_CreateButton(buttonContainer, "Return to Menu", ReturnToMenuClicked);

    currentMusicVolume = AUD_GetMusicVolume();
    AUD_SetMusicVolume(currentMusicVolume / 2);
}

static void Draw(void)
{
    REN_DrawTextureF(pause, pauseLocation);
    BC_DrawButtonContainer(buttonContainer);
}

static void Destroy(void)
{
    AUD_SetMusicVolume(currentMusicVolume);

    REN_FreeTexture(pause);
    BC_DestroyContainer(buttonContainer);
}

static void EnterKeyPressed(void)
{
    BC_ClickSelectedItem(buttonContainer);
}

static void PauseKeyPressed(void)
{
    ResumeClicked();
}

static void ResumeClicked(void)
{
    GAM_PopSceneLayer();
    GAM_SetIsPaused(false);
}

static void ReturnToMenuClicked(void)
{
    GAM_TransitionToScene(&MenuScene);
    GAM_SetIsPaused(false);
}

static void UpKeyClicked(void)
{
    BC_ChangeSelectedIndex(buttonContainer, UP);
}

static void DownKeyClicked(void)
{
    BC_ChangeSelectedIndex(buttonContainer, DOWN);
}
