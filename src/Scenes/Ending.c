//
// Created by Anthony Lesch on 8/2/24.
//

#include "Audio.h"
#include "Game.h"
#include "Renderer.h"
#include "Resources.h"
#include "Text.h"
#include "Utils.h"

#define TEXT_PADDING 15

//----------------------------------------------------------------------------------
// Ending Screen Variables
//----------------------------------------------------------------------------------
static Texture* background;
static Texture* gameOver;
static Texture* message;
static Texture* restart;
static Vector2D gameOverTextPosition;
static Vector2D messageTextPosition;
static Vector2D restartMessagePosition;

//----------------------------------------------------------------------------------
// Ending Screen Functions
//----------------------------------------------------------------------------------
static void Initialize(void);
static void Draw(void);
static void Destroy(void);

//----------------------------------------------------------------------------------
// Ending Scene Extern
//----------------------------------------------------------------------------------
Scene EndingScene = {
    .Initialize = Initialize,
    .Draw = Draw,
    .Destroy = Destroy
};

void Initialize(void)
{
    background = REN_LoadTexture(BACKGROUND_IMAGE);

    int windowWidth, windowHeight;
    GAM_GetScreenDimensions(&windowWidth, &windowHeight);

    const char* text = GAM_GetGameWon() ? "You Won!" : "You Lost!";

    gameOver = TXT_CreateTextureFromText_Large("GAME OVER");
    message = TXT_CreateTextureFromText(text);
    restart = TXT_CreateTextureFromText("Press the spacebar to play again!");

    gameOverTextPosition = UTL_MakeVector2D(windowWidth / 2.f - gameOver->Width / 2.f, windowHeight / 5.f);
    messageTextPosition = UTL_MakeVector2D(windowWidth / 2.f - message->Width / 2.f, gameOverTextPosition.Y + gameOver->Height + TEXT_PADDING);
    restartMessagePosition = UTL_MakeVector2D(windowWidth / 2.f - restart->Width / 2.f, messageTextPosition.Y + message->Height + TEXT_PADDING);

    AUD_PlayMusic(GAMEOVER_MUSIC);
}

void Draw(void)
{
    REN_DrawTexture_Alpha(background, UTL_GetZeroVector(), BB_BACKGROUND_ALPHA);
    REN_DrawTexture(gameOver, gameOverTextPosition);
    REN_DrawTexture(message, messageTextPosition);
    REN_DrawTexture(restart, restartMessagePosition);
}

void Destroy(void)
{
    REN_FreeTexture(background);
    REN_FreeTexture(gameOver);
    REN_FreeTexture(message);
    REN_FreeTexture(restart);
}
