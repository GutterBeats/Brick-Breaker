//
// Created by Anthony Lesch on 7/16/24.
//

#include "Audio.h"
#include "BrickManager.h"
#include "Keyboard.h"
#include "Entity.h"
#include "Game.h"
#include "Resources.h"
#include "Utils.h"

#define DEFAULT_PLAYER_SPEED 350.f
#define DEFAULT_BALL_SPEED 250.f
#define DEFAULT_MAX_LIVES 3
#define DEFAULT_BALL_DAMAGE 30
#define WALL_COUNT 4

//----------------------------------------------------------------------------------
// Gameplay Screen Variables
//----------------------------------------------------------------------------------
static float windowWidth = 0;
static float windowHeight = 0;

static Entity* paddle;
static Entity* ball;
static BrickManager* brickManager;
static Entity** walls;

static i8 paddleCollisionSfx;
static i8 brickCollisionSfx;
static int lives;
static bool shouldFinish;

//----------------------------------------------------------------------------------
// Gameplay Helper Functions
//----------------------------------------------------------------------------------
static void InitializeEntities(void);
static void DestroyWalls(void);
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

    for (int i = 0; i < WALL_COUNT; ++i)
    {
        Entity* wall = walls[i];
        if (ENT_HasCollision(paddle, wall))
        {
            SDL_Log("Entity %s and %s collided!", paddle->Name, wall->Name);

            ENT_ResolveCollision(paddle, wall);
        }
    }
}

void DrawGameplayScreen(void)
{
    ENT_DrawEntity(ball);
    ENT_DrawEntity(paddle);
    BM_DrawBricks(brickManager);

    for (int i = 0; i < WALL_COUNT; ++i)
    {
        ENT_DrawEntity(walls[i]);
    }
}

void UnloadGameplayScreen(void)
{
    ENT_DestroyEntity(paddle);
    ENT_DestroyEntity(ball);
    BM_DestroyManager(brickManager);
    DestroyWalls();
}

bool FinishGameplayScreen(void)
{
    return shouldFinish;
}

void GameplayEnterKeyPressed(void)
{
    if (ball->IsEnabled) return;

    ball->CurrentPosition.X = paddle->CurrentPosition.X + paddle->Size.X / 2;
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

    ball->Name = "Ball";
    ball->Speed = DEFAULT_BALL_SPEED;
    ball->DamageGiven = DEFAULT_BALL_DAMAGE;

    walls = calloc(WALL_COUNT, sizeof(Entity*));
    if (walls == NULL)
    {
        SDL_Log("Could not get memory for walls!");
        return;
    }

    // Left
    walls[0] = ENT_CreateInvisibleEntity(
        UTL_MakeVectorF2D(-windowWidth, 0), UTL_MakeVectorF2D(windowWidth, windowHeight));
    walls[0]->IsEnabled = true;
    walls[0]->Name = "Left Wall";

    // Top
    walls[1] = ENT_CreateInvisibleEntity(
        UTL_GetZeroVectorF(), UTL_MakeVectorF2D(windowWidth, 10));
    walls[1]->IsEnabled = true;
    walls[1]->Name = "Top Wall";

    // Right
    walls[2] = ENT_CreateInvisibleEntity(
        UTL_MakeVectorF2D(windowWidth - 10, 0), UTL_MakeVectorF2D(10, windowHeight));
    walls[2]->IsEnabled = true;
    walls[2]->Name = "Right Wall";

    // Bottom
    walls[3] = ENT_CreateInvisibleEntity(
        UTL_MakeVectorF2D(0, windowHeight - 10), UTL_MakeVectorF2D(windowWidth, 10));
    walls[3]->IsEnabled = true;
    walls[3]->Name = "Bottom Wall";
}

static void DestroyWalls(void)
{
    for (int i = 0; i < WALL_COUNT; ++i)
    {
        ENT_DestroyEntity(walls[i]);
    }

    free(walls);
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

    const VectorF2D currentPosition = ball->CurrentPosition;
    const float ballDiameter = ball->Size.X;

    if (currentPosition.Y + ballDiameter >= windowHeight)
    {
        BallDied();
        return;
    }

    VectorF2D currentDirection = ENT_GetDirection(ball);

    if (currentPosition.X + ballDiameter >= windowWidth || currentPosition.X <= 0.f)
    {
        currentDirection.X *= -1;
    }

    if (currentPosition.Y <= 0.f)
    {
        currentDirection.Y *= -1;
    }

    if (ENT_HasCollision(ball, paddle))
    {
        SDL_Log("Entity %s and %s collided!", ball->Name, paddle->Name);

        ENT_ResolveCollision(ball, paddle);
        if (paddle->PreviousOverlap.X > 0)
        {
            currentDirection.Y *= -1;
        }

        if (paddle->PreviousOverlap.Y > 0)
        {
            currentDirection.X *= -1;
        }

        AUD_PlaySoundEffect(paddleCollisionSfx);
    }

    size_t collisionIndex;
    if (BM_CheckBrickCollision(brickManager, ball, &collisionIndex))
    {
        const Entity* brick = brickManager->Bricks[collisionIndex];
        SDL_Log("Entity %s and %s collided!", ball->Name, brick->Name);

        ENT_ResolveCollision(ball, brick);

        if (brick->PreviousOverlap.X > 0)
        {
            currentDirection.Y *= -1;
        }

        if (brick->PreviousOverlap.Y > 0)
        {
            currentDirection.X *= -1;
        }

        AUD_PlaySoundEffect(brickCollisionSfx);
    }

    ENT_MoveEntity(ball, currentDirection, deltaTime);

    ball->CurrentPosition.X = UTL_FClamp(0, windowWidth - ballDiameter, ball->CurrentPosition.X);
    ball->CurrentPosition.Y = UTL_FClamp(0, windowHeight - ballDiameter, ball->CurrentPosition.Y);
}

static void BallDied(void)
{
    ball->IsEnabled = false;
    shouldFinish = --lives == 0;
}
