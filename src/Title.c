//
// Created by Anthony Lesch on 7/16/24.
//

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
    FADEIN, HOLD, FADEOUT
};

//----------------------------------------------------------------------------------
// Title Screen Variables
//----------------------------------------------------------------------------------
static Texture* companyImage;
static Texture* engineImage;
static float currentAlpha;
static bool shouldFinish;
static enum title_state currentState;
static enum title_frame currentFrame;

//----------------------------------------------------------------------------------
// Title Screen Function Declaration
//----------------------------------------------------------------------------------
static Texture* LoadTitleTexture(const char* path);

void InitTitleScreen(void)
{
    companyImage = LoadTitleTexture(COMPANY_IMAGE);
    engineImage = LoadTitleTexture(ENGINE_IMAGE);

    currentAlpha = 0.f;
    shouldFinish = false;
    currentState = FADEIN;
    currentFrame = COMPANY;
}

void UpdateTitleScreen(const float deltaTime)
{
    static float currentFrameSeconds = 0.f;

    switch (currentState)
    {
        case FADEIN:
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
                currentState = FADEOUT;
            }
            break;
        }
        case FADEOUT:
        {
            currentFrameSeconds -= deltaTime;
            currentAlpha = UTL_FClamp(0.f, 1.f, currentFrameSeconds / FADE_SECONDS);

            if (currentFrameSeconds <= 0.f)
            {
                currentFrameSeconds = 0.f;

                switch (currentFrame)
                {
                    case COMPANY:
                        currentState = FADEIN;
                        currentFrame = ENGINE;
                        break;
                    case ENGINE:
                        shouldFinish = true;
                        break;
                }
            }
            break;
        }
    }
}

void DrawTitleScreen(void)
{
    int screenWidth, screenHeight;
    GetScreenDimensions(&screenWidth, &screenHeight);

    switch (currentFrame)
    {
        case COMPANY:
        {
            const int xPos = screenWidth / 2 - companyImage->Width / 2;
            const int yPos = screenHeight / 2 - companyImage->Height / 2;

            DrawTexture_Alpha(companyImage, xPos, yPos, currentAlpha);
            break;
        }
        case ENGINE:
        {
            const int xPos = screenWidth / 2 - engineImage->Width / 2;
            const int yPos = screenHeight / 2 - engineImage->Height / 2;

            DrawTexture_Alpha(engineImage, xPos, yPos, currentAlpha);
            break;
        }
    }
}

void UnloadTitleScreen(void)
{
    FreeTexture(companyImage);
    FreeTexture(engineImage);
}

bool FinishTitleScreen(void)
{
    return shouldFinish;
}

void TitleEnterKeyPressed(void)
{
    SDL_Log("Enter key pressed on the title screen.");
}

static Texture* LoadTitleTexture(const char* path)
{
    Texture* titleTexture = LoadTexture(path);
    if (titleTexture == NULL)
    {
        shouldFinish = true;
        return NULL;
    }

    titleTexture->Width -= IMAGE_PADDING;
    titleTexture->Height -= IMAGE_PADDING;

    int screenWidth, screenHeight;
    GetScreenDimensions(&screenWidth, &screenHeight);

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
