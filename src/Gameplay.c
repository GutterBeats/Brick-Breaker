//
// Created by Anthony Lesch on 7/16/24.
//

#include <math.h>

#include "BrickManager.h"
#include "Keyboard.h"
#include "Entity.h"
#include "Game.h"
#include "Utils.h"

#define PLAYER_TEXTURE "resources/Textures/paddleBlue.png"
#define BALL_TEXTURE "resources/Textures/ballBlue.png"

//----------------------------------------------------------------------------------
// Gameplay Screen Variables
//----------------------------------------------------------------------------------
static float windowWidth = 0;
static float windowHeight = 0;
static float interpolationSpeed = 0.9f;
static float playerMovementSpeed = 350.f;
static float ballSpeedX = 200.f;
static float ballSpeedY = 200.f;
static float destinationX;
static size_t brickCount;

static Entity* player;
static Entity* ball;
static Brick** bricks;

//----------------------------------------------------------------------------------
// Gameplay Helper Functions
//----------------------------------------------------------------------------------
static void UpdatePlayerPosition(float deltaTime);
static void UpdateBallPosition(float deltaTime);
static void DrawBricks(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions
//----------------------------------------------------------------------------------
void InitGameplayScreen(void)
{
    int width, height;
    GetScreenDimensions(&width, &height);

    windowWidth = (float)width;
    windowHeight = (float)height;

    player = CreateEntity(
            windowWidth / 2.f, windowHeight,
            PLAYER_TEXTURE);

    ball = CreateEntity(
            windowWidth / 2.f, windowHeight / 2.f,
            BALL_TEXTURE);

    ASSERT_NOTNULL(player, "Player")
    ASSERT_NOTNULL(ball, "Ball")

    bricks = CreateBricks(50, 50, windowWidth - 50, windowHeight - 200, &brickCount);

    const float playerWidth = player->Bounds.w;
    destinationX = windowWidth / 2.f - playerWidth / 2.f;

    player->Bounds.y -= player->Bounds.h * 2.f;
    player->Bounds.x = destinationX;
    player->IsEnabled = true;
}

void UpdateGameplayScreen(const float deltaTime)
{
    UpdatePlayerPosition(deltaTime);
    UpdateBallPosition(deltaTime);
}

void DrawGameplayScreen(void)
{
    DrawEntity(ball, NULL);
    DrawEntity(player, NULL);
    DrawBricks();
}

void UnloadGameplayScreen(void)
{
    DestroyEntity(player);
    DestroyEntity(ball);
    DestroyBricks(bricks, brickCount);
}

bool FinishGameplayScreen(void)
{
    return false;
}

void GameplayEnterKeyPressed(void)
{
    ball->IsEnabled = true;
}

static void DrawBricks(void)
{
    for (int i = 0; i < brickCount; ++i)
    {
        DrawEntity(bricks[i]->Entity, NULL);
    }
}

static void UpdatePlayerPosition(const float deltaTime)
{
    if (KBD_IsLeftKeyDown())
    {
        destinationX -= playerMovementSpeed * deltaTime;
    }
    else if (KBD_IsRightKeyDown())
    {
        destinationX += playerMovementSpeed * deltaTime;
    }

    const float playerWidth = player->Bounds.w;
    destinationX = UTL_FClamp(0.f, windowWidth - playerWidth, destinationX);

    const float interpolation = powf(interpolationSpeed, deltaTime * 60);
    player->Bounds.x = UTL_Lerp(destinationX, player->Bounds.x, interpolation);
}

static void UpdateBallPosition(const float deltaTime)
{
    if (!ball->IsEnabled) return;

    const float ballDiameter = ball->Bounds.w;

    if (ball->Bounds.x + ballDiameter >= windowWidth || ball->Bounds.x <= 0.f)
    {
        ballSpeedX *= -1;
    }

    if (ball->Bounds.y + ballDiameter >= windowHeight || ball->Bounds.y <= 0.f)
    {
        ballSpeedY *= -1;
    }

    if (HasTopCollision(ball, player))
    {
        ballSpeedY *= -1;
    }



    ball->Bounds.x += ballSpeedX * deltaTime;
    ball->Bounds.x = UTL_FClamp(0, windowWidth - ballDiameter, ball->Bounds.x);

    ball->Bounds.y += ballSpeedY * deltaTime;
    ball->Bounds.y = UTL_FClamp(0, windowHeight - ballDiameter, ball->Bounds.y);
}
