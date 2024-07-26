//
// Created by Anthony Lesch on 7/16/24.
//

#include <math.h>

#include "Audio.h"
#include "BrickManager.h"
#include "Keyboard.h"
#include "Entity.h"
#include "Game.h"
#include "Resources.h"
#include "Utils.h"

#define DEFAULT_BALL_SPEED 200.f

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

static Entity* player;
static Entity* ball;
static BrickManager* brickManager;

//----------------------------------------------------------------------------------
// Gameplay Helper Functions
//----------------------------------------------------------------------------------
static void UpdatePlayerPosition(float deltaTime);
static void UpdateBallPosition(float deltaTime);

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
            PLAYER_RED_TEXTURE);

    ball = CreateEntity(
            windowWidth / 2.f, windowHeight / 2.f,
            BALL_BLUE_TEXTURE);

    brickManager = CreateBricks(90, 50, windowWidth - 100, windowHeight / 3, ball->Bounds.h * .5f);

    ASSERT_NOTNULL(player, "Player")
    ASSERT_NOTNULL(ball, "Ball")
    ASSERT_NOTNULL(brickManager, "Brick Manager")

    const float playerWidth = player->Bounds.w;
    destinationX = windowWidth / 2.f - playerWidth / 2.f;

    player->Bounds.y -= player->Bounds.h * 2.f;
    player->Bounds.x = destinationX;
    player->IsEnabled = true;

    PlayMusic(MAIN_MUSIC);
}

void UpdateGameplayScreen(const float deltaTime)
{
    UpdatePlayerPosition(deltaTime);
    UpdateBallPosition(deltaTime);
}

void DrawGameplayScreen(void)
{
    DrawEntity(ball);
    DrawEntity(player);
    DrawBricks(brickManager);
}

void UnloadGameplayScreen(void)
{
    DestroyEntity(player);
    DestroyEntity(ball);
    DestroyManager(brickManager);
}

bool FinishGameplayScreen(void)
{
    return false;
}

void GameplayEnterKeyPressed(void)
{
    if (ball->IsEnabled) return;

    ball->Bounds.x = player->Bounds.x + player->Bounds.w / 2;
    ball->Bounds.y = player->Bounds.y - player->Bounds.h;

    const Vector2D up = GetUpVector();

    ballSpeedY = up.Y * DEFAULT_BALL_SPEED;

    // TODO: Calculate this as a random direction between left and right
    ballSpeedX = DEFAULT_BALL_SPEED;

    ball->IsEnabled = true;
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

    const SDL_FRect bounds = ball->Bounds;
    const float ballDiameter = bounds.w;

    if (bounds.x + ballDiameter >= windowWidth || bounds.x <= 0.f)
    {
        ballSpeedX *= -1;
    }

    if (bounds.y + ballDiameter >= windowHeight || bounds.y <= 0.f)
    {
        ballSpeedY *= -1;
    }

    if (HasTopCollision(ball, player))
    {
        ballSpeedY *= -1;
    }

    if (CheckBrickCollision(brickManager, ball))
    {
        ballSpeedY *= -1;
    }

    ball->Bounds.x += ballSpeedX * deltaTime;
    ball->Bounds.x = UTL_FClamp(0, windowWidth - ballDiameter, ball->Bounds.x);

    ball->Bounds.y += ballSpeedY * deltaTime;
    ball->Bounds.y = UTL_FClamp(0, windowHeight - ballDiameter, ball->Bounds.y);
}
