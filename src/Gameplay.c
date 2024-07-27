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
            (VectorF2D){ windowWidth / 2.f, windowHeight},
            PLAYER_RED_TEXTURE);

    ball = CreateEntity(
            (VectorF2D){ windowWidth / 2.f, windowHeight / 2.f },
            BALL_BLUE_TEXTURE);

    brickManager = CreateBricks(
        (VectorF2D){90, 50 },
        (VectorF2D){ windowWidth - 100, windowHeight / 3 },
        ball->Size.Y * .5f);

    ASSERT_NOTNULL(player, "Player")
    ASSERT_NOTNULL(ball, "Ball")
    ASSERT_NOTNULL(brickManager, "Brick Manager")

    const float playerWidth = player->Size.X;
    destinationX = windowWidth / 2.f - playerWidth / 2.f;

    player->CurrentPosition.Y -= player->Size.Y * 2.f;
    player->CurrentPosition.X = destinationX;
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

    ball->CurrentPosition.X = player->CurrentPosition.X + player->Size.X / 2;
    ball->CurrentPosition.Y = player->CurrentPosition.Y - player->Size.Y;

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

    const float playerWidth = player->Size.X;
    destinationX = UTL_FClamp(0.f, windowWidth - playerWidth, destinationX);

    const float interpolation = powf(interpolationSpeed, deltaTime * 60);
    player->CurrentPosition.X = UTL_Lerp(destinationX, player->CurrentPosition.X, interpolation);
}

static void UpdateBallPosition(const float deltaTime)
{
    if (!ball->IsEnabled) return;

    const VectorF2D currentPosition = ball->CurrentPosition;
    const float ballDiameter = ball->Size.X;

    if (currentPosition.X + ballDiameter >= windowWidth || currentPosition.X <= 0.f)
    {
        ballSpeedX *= -1;
    }

    if (currentPosition.Y + ballDiameter >= windowHeight || currentPosition.Y <= 0.f)
    {
        ballSpeedY *= -1;
    }

    if (HasTopCollision(ball, player))
    {
        ballSpeedY *= -1;

        if (paddleCollisionSfx >= 0)
        {
            PlaySoundEffect(paddleCollisionSfx);
        }
    }

    const Entity collision;
    if (CheckBrickCollision(brickManager, ball, &collision))
    {

        ballSpeedY *= -1;
    }

    ball->CurrentPosition.X += ballSpeedX * deltaTime;
    ball->CurrentPosition.X = UTL_FClamp(0, windowWidth - ballDiameter, ball->CurrentPosition.X);

    ball->CurrentPosition.Y += ballSpeedY * deltaTime;
    ball->CurrentPosition.Y = UTL_FClamp(0, windowHeight - ballDiameter, ball->CurrentPosition.Y);
}
