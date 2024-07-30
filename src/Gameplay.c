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

#define DEFAULT_BALL_SPEED 250.f
#define DEFAULT_MAX_LIVES 3
#define DEFAULT_BALL_DAMAGE 30

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

static Entity* paddle;
static Entity* ball;
static BrickManager* brickManager;

static i8 paddleCollisionSfx;
static i8 brickCollisionSfx;
static int lives;

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
    GAM_GetScreenDimensions(&width, &height);

    windowWidth = (float)width;
    windowHeight = (float)height;

    paddle = ENT_CreateEntity(
            (VectorF2D){ windowWidth / 2.f, windowHeight},
            PLAYER_RED_TEXTURE);

    ball = ENT_CreateEntity(
            (VectorF2D){ windowWidth / 2.f, windowHeight / 2.f },
            BALL_BLUE_TEXTURE);

    brickManager = BM_CreateBricks(
        (VectorF2D){90, 50 },
        (VectorF2D){ windowWidth - 100, windowHeight / 3 },
        ball->Size.Y * .5f);

    ASSERT_NOTNULL(paddle, "Player")
    ASSERT_NOTNULL(ball, "Ball")
    ASSERT_NOTNULL(brickManager, "Brick Manager")

    const float playerWidth = paddle->Size.X;
    destinationX = windowWidth / 2.f - playerWidth / 2.f;

    paddle->Position.Y -= paddle->Size.Y * 2.f;
    paddle->Position.X = destinationX;
    paddle->IsEnabled = true;

    paddleCollisionSfx = AUD_LoadSoundEffect(PADDLE_COLLISION_SFX);
    brickCollisionSfx = AUD_LoadSoundEffect(BRICK_COLLISION_SFX);
    lives = DEFAULT_MAX_LIVES;

    AUD_PlayMusic(MAIN_MUSIC);
}

void UpdateGameplayScreen(const float deltaTime)
{
    UpdatePlayerPosition(deltaTime);
    UpdateBallPosition(deltaTime);
}

void DrawGameplayScreen(void)
{
    ENT_DrawEntity(ball);
    ENT_DrawEntity(paddle);
    BM_DrawBricks(brickManager);
}

void UnloadGameplayScreen(void)
{
    ENT_DestroyEntity(paddle);
    ENT_DestroyEntity(ball);
    BM_DestroyManager(brickManager);
}

bool FinishGameplayScreen(void)
{
    return false;
}

void GameplayEnterKeyPressed(void)
{
    if (ball->IsEnabled) return;

    ball->Position.X = paddle->Position.X + paddle->Size.X / 2;
    ball->Position.Y = paddle->Position.Y - paddle->Size.Y;

    const Vector2D up = UTL_GetUpVector();

    ballSpeedY = up.Y * DEFAULT_BALL_SPEED;

    // TODO: Calculate this as a random direction between left and right
    ballSpeedX = DEFAULT_BALL_SPEED;

    ball->IsEnabled = true;
    ball->DamageGiven = DEFAULT_BALL_DAMAGE;
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

    const float playerWidth = paddle->Size.X;
    destinationX = UTL_FClamp(0.f, windowWidth - playerWidth, destinationX);

    const float interpolation = powf(interpolationSpeed, deltaTime * 60);
    paddle->Position.X = UTL_Lerp(destinationX, paddle->Position.X, interpolation);
}

static void UpdateBallPosition(const float deltaTime)
{
    if (!ball->IsEnabled) return;

    const VectorF2D currentPosition = ball->Position;
    const float ballDiameter = ball->Size.X;

    if (currentPosition.Y + ballDiameter >= windowHeight)
    {
        --lives;
        ball->IsEnabled = false;

        return;
    }

    if (currentPosition.X + ballDiameter >= windowWidth || currentPosition.X <= 0.f)
    {
        ballSpeedX *= -1;
    }

    if (currentPosition.Y <= 0.f)
    {
        ballSpeedY *= -1;
    }

    if (ENT_HasCollision(ball, paddle))
    {
        ENT_ResolveCollision(ball, paddle);
        if (paddle->PreviousOverlap.X > 0)
        {
            ballSpeedY *= -1;
        }

        if (paddle->PreviousOverlap.Y > 0)
        {
            ballSpeedX *= -1;
        }

        AUD_PlaySoundEffect(paddleCollisionSfx);
    }

    size_t collisionIndex;
    if (BM_CheckBrickCollision(brickManager, ball, &collisionIndex))
    {
        const Entity* collision = brickManager->Bricks[collisionIndex];
        ENT_ResolveCollision(ball, collision);

        if (collision->PreviousOverlap.X > 0)
        {
            ballSpeedY *= -1;
        }

        if (collision->PreviousOverlap.Y > 0)
        {
            ballSpeedX *= -1;
        }

        AUD_PlaySoundEffect(brickCollisionSfx);
    }

    ball->Position.X += ballSpeedX * deltaTime;
    ball->Position.X = UTL_FClamp(0, windowWidth - ballDiameter, ball->Position.X);

    ball->Position.Y += ballSpeedY * deltaTime;
    ball->Position.Y = UTL_FClamp(0, windowHeight - ballDiameter, ball->Position.Y);
}
