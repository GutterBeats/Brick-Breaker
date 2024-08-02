//
// Created by Anthony Lesch on 7/16/24.
//

#include "Audio.h"
#include "BrickManager.h"
#include "Collision.h"
#include "Entity.h"
#include "Game.h"
#include "Keyboard.h"
#include "Resources.h"
#include "Utils.h"

#define DEFAULT_PLAYER_SPEED 350.f
#define DEFAULT_BALL_SPEED 250.f
#define DEFAULT_MAX_LIVES 3
#define DEFAULT_BALL_DAMAGE 30

//----------------------------------------------------------------------------------
// Gameplay Screen Variables
//----------------------------------------------------------------------------------
static float windowWidth = 0;
static float windowHeight = 0;

static Entity* paddle;
static Entity* ball;
static BrickManager* brickManager;

static i8 paddleCollisionSfx;
static i8 brickCollisionSfx;
static int lives;
static bool shouldFinish;

//----------------------------------------------------------------------------------
// Gameplay Helper Functions
//----------------------------------------------------------------------------------
static void InitializeEntities(void);
static void UpdatePlayerPosition(float deltaTime);
static void UpdateBallPosition(float deltaTime);
static void BallDied(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions
//----------------------------------------------------------------------------------
void InitGameplayScreen(void)
{
    int width, height;
    GAM_GetScreenDimensions(&width, &height);

    shouldFinish = false;
    windowWidth = (float)width;
    windowHeight = (float)height;

    paddleCollisionSfx = AUD_LoadSoundEffect(PADDLE_COLLISION_SFX);
    brickCollisionSfx = AUD_LoadSoundEffect(BRICK_COLLISION_SFX);
    lives = DEFAULT_MAX_LIVES;

    InitializeEntities();

    //AUD_PlayMusic(MAIN_MUSIC);
}

void UpdateGameplayScreen(const float deltaTime)
{
    UpdatePlayerPosition(deltaTime);
    UpdateBallPosition(deltaTime);

    paddle->CurrentPosition.X = UTL_FClamp(0, windowWidth - paddle->Size.X, paddle->CurrentPosition.X);
    ball->CurrentPosition.X = UTL_FClamp(0, windowWidth - ball->Size.X / 2.f, ball->CurrentPosition.X);
    ball->CurrentPosition.Y = UTL_FClamp(0, windowHeight - ball->Size.Y / 2.f, ball->CurrentPosition.Y);

    if (ball->CurrentPosition.Y + ball->Size.Y >= windowHeight)
    {
        BallDied();
    }
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
    return shouldFinish;
}

void GameplayEnterKeyPressed(void)
{
    if (ball->IsEnabled) return;

    ball->CurrentPosition.X = paddle->CurrentPosition.X + paddle->Size.X / 2.f;
    ball->CurrentPosition.Y = paddle->CurrentPosition.Y - paddle->Size.Y;
    ball->PreviousPosition = ball->CurrentPosition;
    ball->IsEnabled = true;

    ENT_MoveEntity(ball, UTL_GetUpVectorF(), GAM_GetDeltaSeconds());
}

static void InitializeEntities(void)
{
    paddle = ENT_CreateEntity(
                UTL_MakeVectorF2D(windowWidth / 2.f, windowHeight),
                PLAYER_RED_TEXTURE);

    ball = ENT_CreateEntity(
            UTL_MakeVectorF2D(windowWidth / 2.f, windowHeight / 2.f),
            BALL_BLUE_TEXTURE);

    brickManager = BM_CreateBricks(
        UTL_MakeVectorF2D(90, 50),
        UTL_MakeVectorF2D(windowWidth - 100, windowHeight / 3),
        ball->Size.Y * .5f);

    ASSERT_NOTNULL(paddle, "Player")
    ASSERT_NOTNULL(ball, "Ball")
    ASSERT_NOTNULL(brickManager, "Brick Manager")

    paddle->CurrentPosition.Y -= paddle->Size.Y * 2.f;
    paddle->CurrentPosition.X = windowWidth / 2.f - paddle->Size.X / 2.f;
    paddle->Speed = DEFAULT_PLAYER_SPEED;
    paddle->IsEnabled = true;
    paddle->Name = "Player Paddle";
    paddle->CollisionVolume = COL_MakeBoxCollisionVolume(paddle->CurrentPosition, paddle->Size);

    ball->Name = "Ball";
    ball->Speed = DEFAULT_BALL_SPEED;
    ball->DamageGiven = DEFAULT_BALL_DAMAGE;
    ball->CollisionVolume = COL_MakeCircleCollisionVolume(paddle->CurrentPosition, ball->Size.X / 2.f);
}

static void UpdatePlayerPosition(const float deltaTime)
{
    if (KBD_IsLeftKeyDown())
    {
        ENT_MoveEntity(paddle, UTL_GetLeftVectorF(), deltaTime);
    }

    if (KBD_IsRightKeyDown())
    {
        ENT_MoveEntity(paddle, UTL_GetRightVectorF(), deltaTime);
    }
}

static void UpdateBallPosition(const float deltaTime)
{
    if (!ball->IsEnabled) return;

    VectorF2D currentDirection = ENT_GetDirection(ball);

    const CollisionResult paddleResult = COL_HasCollisionBoxCircle(paddle->CollisionVolume, ball->CollisionVolume);
    const CollisionResult brickResult = BM_CheckBrickCollision(brickManager, ball);

    if (paddleResult.Collided)
    {
        switch (paddleResult.Direction)
        {
            case UP:
                ball->CurrentPosition.Y += paddleResult.Difference.Y;
                currentDirection.Y *= -1;
                break;
            case DOWN:
                ball->CurrentPosition.Y -= paddleResult.Difference.Y;
                currentDirection.Y *= -1;
            case LEFT:
                ball->CurrentPosition.X += paddleResult.Difference.X;
                currentDirection.X *= -1;
                break;
            case RIGHT:
                ball->CurrentPosition.X += paddleResult.Difference.X;
                currentDirection.X *= -1;
                break;
            default: break;
        }

        /*
        *  // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        // fix sticky paddle
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
         */

        const float centerPaddle = paddle->CurrentPosition.X + paddle->Size.X / 2.f;
        const float distance = ball->CurrentPosition.X + ball->Size.X / 2.f - centerPaddle;
        const float percentage = distance / (paddle->Size.X / 2.f);
        const float strength = 2.0f;

        if (currentDirection.X == 0)
        {
            currentDirection.X += percentage * strength;
        }
        else
        {
            currentDirection.X *= percentage * strength;
        }

        AUD_PlaySoundEffect(paddleCollisionSfx);
    }

    if (brickResult.Collided)
    {
        switch (brickResult.Direction)
        {
            case UP:
                ball->CurrentPosition.Y -= brickResult.Difference.Y;
                currentDirection.Y *= -1;
                break;
            case DOWN:
                ball->CurrentPosition.Y += brickResult.Difference.Y;
                currentDirection.Y *= -1;
            case LEFT:
                ball->CurrentPosition.X += brickResult.Difference.X;
                currentDirection.X *= -1;
                break;
            case RIGHT:
                ball->CurrentPosition.X -= brickResult.Difference.X;
                currentDirection.X *= -1;
                break;
            default: break;
        }

        AUD_PlaySoundEffect(brickCollisionSfx);
    }

    ENT_MoveEntity(ball, currentDirection, deltaTime);

    const VectorF2D ballPosition = ball->CurrentPosition;
    if (ballPosition.X <= 0 || ballPosition.X + ball->Size.X >= windowWidth)
    {
        currentDirection.X *= 1;
    }

    if (ballPosition.Y <= 0)
    {
        currentDirection.Y *= 1;
    }

    ENT_MoveEntity(ball, currentDirection, deltaTime);
}

static void BallDied(void)
{
    if (!ball->IsEnabled) return;

    ball->IsEnabled = false;
    shouldFinish = --lives == 0;
}
