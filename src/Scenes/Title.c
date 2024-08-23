//
// Created by Anthony Lesch on 7/16/24.
//

#include "EventSystem.h"
#include "Game.h"
#include "Renderer.h"
#include "Resources.h"
#include "Utils.h"

#define FADE_SECONDS 1.5f
#define IMAGE_PADDING 50

enum title_frame
{
    COMPANY, ENGINE
};

enum title_state
{
    FADE_IN, HOLD, FADE_OUT
};

//----------------------------------------------------------------------------------
// Title Screen Variables
//----------------------------------------------------------------------------------
static Texture* companyImage;
static Texture* engineImage;
static float currentAlpha;
static enum title_state currentState;
static enum title_frame currentFrame;

//----------------------------------------------------------------------------------
// Title Screen Function Declaration
//----------------------------------------------------------------------------------
static void Initialize(void);
static void Update(float deltaTime);
static void Draw(void);
static void Destroy(void);
static void EnterKeyPressed(void);
static Texture* LoadTitleTexture(const char* path);

//----------------------------------------------------------------------------------
// Title Screen Extern
//----------------------------------------------------------------------------------
Scene TitleScene = {
    .Name = "TITLE SCENE",
    .Initialize = Initialize,
    .Update = Update,
    .Draw = Draw,
    .Destroy = Destroy
};

void Initialize(void)
{
    companyImage = LoadTitleTexture(COMPANY_IMAGE);
    engineImage = LoadTitleTexture(ENGINE_IMAGE);

    currentAlpha = 0.f;
    currentState = FADE_IN;
    currentFrame = COMPANY;

    EVT_BindUserEvent(ENTER, EnterKeyPressed);
}

void Update(const float deltaTime)
{
    static float currentFrameSeconds = 0.f;

    switch (currentState)
    {
        case FADE_IN:
        {
            currentFrameSeconds += deltaTime;
            currentAlpha = UTL_FClamp(0.f, 1.f, currentFrameSeconds / FADE_SECONDS);

            if (currentFrameSeconds >= FADE_SECONDS)
            {
                currentFrameSeconds = 0.f;
                currentState = HOLD;
            }
            break;
        }
        case HOLD:
        {
            currentFrameSeconds += deltaTime;
            currentAlpha = 1.f;

            if (currentFrameSeconds >= FADE_SECONDS)
            {
                currentFrameSeconds = FADE_SECONDS;
                currentState = FADE_OUT;
            }
            break;
        }
        case FADE_OUT:
        {
            currentFrameSeconds -= deltaTime;
            currentAlpha = UTL_FClamp(0.f, 1.f, currentFrameSeconds / FADE_SECONDS);

            if (currentFrameSeconds <= 0.f)
            {
                currentFrameSeconds = 0.f;

                switch (currentFrame)
                {
                    case COMPANY:
                        currentState = FADE_IN;
                        currentFrame = ENGINE;
                        break;
                    case ENGINE:
                        GAM_TransitionToScene(&MenuScene);
                        break;
                }
            }
            break;
        }
    }
}

void Draw(void)
{
    int screenWidth, screenHeight;
    GAM_GetScreenDimensions(&screenWidth, &screenHeight);

    switch (currentFrame)
    {
        case COMPANY:
        {
            const Vector2D position = UTL_MakeVector2D(
                screenWidth / 2 - companyImage->Width / 2,
                screenHeight / 2 - companyImage->Height / 2
            );

            REN_DrawTexture_Alpha(companyImage, position, currentAlpha);
            break;
        }
        case ENGINE:
        {
            const Vector2D position = UTL_MakeVector2D(
                screenWidth / 2 - engineImage->Width / 2,
                screenHeight / 2 - engineImage->Height / 2
            );

            REN_DrawTexture_Alpha(engineImage, position, currentAlpha);
            break;
        }
    }
}

void Destroy(void)
{
    REN_FreeTexture(companyImage);
    REN_FreeTexture(engineImage);
}

static void EnterKeyPressed(void)
{
    currentState = FADE_OUT;
}

static Texture* LoadTitleTexture(const char* path)
{
    Texture* titleTexture = REN_LoadTexture(path);
    ASSERT_NOTNULL(titleTexture, "Title Texture")

    titleTexture->Width -= IMAGE_PADDING;
    titleTexture->Height -= IMAGE_PADDING;

    int screenWidth, screenHeight;
    GAM_GetScreenDimensions(&screenWidth, &screenHeight);

    const float imageRatio = (float)titleTexture->Width / (float)titleTexture->Height;

    if (titleTexture->Width > screenWidth)
    {
        titleTexture->Width = screenWidth - IMAGE_PADDING;
        titleTexture->Height = (float)titleTexture->Width * imageRatio;
    }

    if (titleTexture->Height > screenHeight)
    {
        titleTexture->Height = screenHeight - IMAGE_PADDING;
        titleTexture->Width = (float)titleTexture->Height * imageRatio;
    }

    return titleTexture;
}
