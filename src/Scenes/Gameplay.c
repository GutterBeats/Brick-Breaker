//
// Created by Anthony Lesch on 7/16/24.
//

#include "Audio.h"
#include "BrickManager.h"
#include "Collision.h"
#include "Entity.h"
#include "EventSystem.h"
#include "Game.h"
#include "Keyboard.h"
#include "Resources.h"
#include "Text.h"
#include "Utilities.h"

#define DEFAULT_PLAYER_SPEED 550.f
#define DEFAULT_BALL_SPEED 650.f
#define DEFAULT_MAX_LIVES 3
#define DEFAULT_BALL_DAMAGE 30
#define STUCK_FRAME_COUNT 120
#define TEXT_TOP_PADDING 5
#define TEXT_SIDE_PADDING 10

//----------------------------------------------------------------------------------
// Gameplay Screen Variables
//----------------------------------------------------------------------------------
static float windowWidth = 0;
static float windowHeight = 0;

static Texture* background;
static Texture* lifeTexture;
static Texture* scoreTexture;
static Entity* paddle;
static Entity* ball;
static BrickManager* brickManager;

static i8 paddleCollisionSfx;
static i8 brickCollisionSfx;
static int lives;
static bool shouldFinish;
static bool shouldFinishNextFrame;

extern Scene PauseScene;

//----------------------------------------------------------------------------------
// Gameplay Helper Functions
//----------------------------------------------------------------------------------
static void InitializeEntities(void);
static void UpdatePlayerPosition(float deltaTime);
static void UpdateBallPosition(float deltaTime);
static void BallDied(void);
static void UnstickBall(void);
static void EnterKeyPressed(void);
static void PauseKeyPressed(void);
static void DrawScoreAndLives(void);
static void TryFinishGameplayScreen(void);

//----------------------------------------------------------------------------------
// Gameplay Scene Functions
//----------------------------------------------------------------------------------
static void Initialize(void);
static void Update(float deltaTime);
static void Draw(void);
static void Destroy(void);
static void ReturnFromBackground(void);

//----------------------------------------------------------------------------------
// Gameplay Scene Extern
//----------------------------------------------------------------------------------
Scene GameplayScene = {
    .Name = "GAMEPLAY SCENE",
    .Initialize = Initialize,
    .Update = Update,
    .Draw = Draw,
    .Destroy = Destroy,
    .ReturnFromBackground = ReturnFromBackground
};

static void Initialize(void)
{
    int width, height;
    GAM_GetScreenDimensions(&width, &height);

    shouldFinish = false;
    windowWidth = (float)width;
    windowHeight = (float)height;
    shouldFinishNextFrame = false;

    paddleCollisionSfx = AUD_LoadSoundEffect(PADDLE_COLLISION_SFX);
    brickCollisionSfx = AUD_LoadSoundEffect(BRICK_COLLISION_SFX);
    lives = DEFAULT_MAX_LIVES;

    InitializeEntities();

    AUD_PlayMusic(MAIN_MUSIC);
    ReturnFromBackground();

    GAM_UpdateScore(
        -GAM_GetScore());
}

static void Update(const float deltaTime)
{
    if (shouldFinish || GAM_GetIsPaused()) return;

    UpdatePlayerPosition(deltaTime);
    UpdateBallPosition(deltaTime);

    paddle->CurrentPosition.X = UTL_FClamp(0, windowWidth - paddle->Size.X, paddle->CurrentPosition.X);
    ball->CurrentPosition.X = UTL_FClamp(0, windowWidth - ball->Size.X / 2.f, ball->CurrentPosition.X);
    ball->CurrentPosition.Y = UTL_FClamp(0, windowHeight - ball->Size.Y / 2.f, ball->CurrentPosition.Y);

    if (ball->CurrentPosition.Y + ball->Size.Y > paddle->CurrentPosition.Y + paddle->Size.Y)
    {
        BallDied();
        return;
    }

    UnstickBall();

    if (brickManager->EnabledBrickCount <= 0)
    {
        GAM_SetGameWon(true);
        shouldFinishNextFrame = true;
    }
}

static void Draw(void)
{
    REN_DrawTexture_Alpha(background, UTL_GetZeroVector(), BB_BACKGROUND_ALPHA);

    DrawScoreAndLives();

    ENT_DrawEntity(ball);
    ENT_DrawEntity(paddle);
    BM_DrawBricks(brickManager);

    TryFinishGameplayScreen();
}

static void Destroy(void)
{
    ENT_DestroyEntity(paddle);
    ENT_DestroyEntity(ball);
    BM_DestroyManager(brickManager);
    REN_FreeTexture(background);
    REN_FreeTexture(lifeTexture);
    REN_FreeTexture(scoreTexture);

    lifeTexture = NULL;
    scoreTexture = NULL;

    AUD_UnloadSoundEffect(paddleCollisionSfx);
    AUD_UnloadSoundEffect(brickCollisionSfx);
}

static void ReturnFromBackground(void)
{
    EVT_BindUserEvent(ENTER, EnterKeyPressed);
    EVT_BindUserEvent(PAUSE, PauseKeyPressed);
}

static void TryFinishGameplayScreen(void)
{
    if (lives > 0) return;

    BB_LOG("Finishing Gameplay");

    if (shouldFinishNextFrame)
    {
        ball->IsEnabled = false;
        shouldFinishNextFrame = false;

        return;
    }

    GAM_TransitionToScene(&EndingScene);
}

static void EnterKeyPressed(void)
{
    if (ball->IsEnabled) return;

    ball->CurrentPosition.X = paddle->CurrentPosition.X + paddle->Size.X / 2.f;
    ball->CurrentPosition.Y = paddle->CurrentPosition.Y - paddle->Size.Y * 2.f;
    ball->PreviousPosition = ball->CurrentPosition;
    ball->IsEnabled = true;

    ENT_MoveEntity(ball, UTL_GetUpVectorF(), GAM_GetDeltaSeconds());
}

static void PauseKeyPressed(void)
{
    GAM_SetIsPaused(
        !GAM_GetIsPaused());

    if (GAM_GetIsPaused())
    {
        GAM_PushSceneLayer(&PauseScene);
    }
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

    background = REN_LoadTexture(BACKGROUND_IMAGE);

    paddle->CurrentPosition.Y -= paddle->Size.Y * 2.f;
    paddle->CurrentPosition.X = windowWidth / 2.f - paddle->Size.X / 2.f;
    paddle->Speed = DEFAULT_PLAYER_SPEED;
    paddle->IsEnabled = true;
    paddle->Name = "Player Paddle";
    paddle->CollisionVolume = COL_MakeCollisionVolume(paddle->CurrentPosition, paddle->Size);

    ball->Name = "Ball";
    ball->Speed = DEFAULT_BALL_SPEED;
    ball->DamageGiven = DEFAULT_BALL_DAMAGE;
    ball->CollisionVolume = COL_MakeCollisionVolume(paddle->CurrentPosition, ball->Size);
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

    if (paddleResult.Collided)
    {
        switch (paddleResult.Direction)
        {
            case UP:
                ball->CurrentPosition.Y += paddleResult.Difference.Y + 2;
                currentDirection.Y *= -1;
                break;
            case DOWN:
                ball->CurrentPosition.Y -= paddleResult.Difference.Y + 2;
                currentDirection.Y *= -1;
                break;
            case LEFT:
                ball->CurrentPosition.X += paddleResult.Difference.X + 2;
                currentDirection.X *= -1;
                break;
            case RIGHT:
                ball->CurrentPosition.X -= paddleResult.Difference.X + 2;
                currentDirection.X *= -1;
                break;
            default: break;
        }

        const float centerPaddle = paddle->CurrentPosition.X + paddle->Size.X / 2.f;
        const float distance = ball->CurrentPosition.X + ball->Size.X / 2.f - centerPaddle;
        const float percentage = UTL_FClamp(0.001f, 100.f, distance / (paddle->Size.X / 2.f));

        const VectorF2D vector = ball->CurrentPosition.X > centerPaddle ? UTL_GetRightVectorF() : UTL_GetLeftVectorF();

        if (currentDirection.X == 0)
        {
            currentDirection.X += vector.X / (percentage * 2.f);
        }
        else
        {
            currentDirection.X *= vector.X / (percentage * 2.f);
        }

        AUD_PlaySoundEffect(paddleCollisionSfx);
    }

    const CollisionResult brickResult = BM_CheckBrickCollision(brickManager, ball);

    if (brickResult.Collided)
    {
        switch (brickResult.Direction)
        {
            case UP:
                ball->CurrentPosition.Y -= brickResult.Difference.Y + ball->Size.Y / 2.f;
                currentDirection.Y *= -1;
                break;
            case DOWN:
                ball->CurrentPosition.Y += brickResult.Difference.Y + ball->Size.Y / 2.f;
                currentDirection.Y *= -1;
            case LEFT:
                ball->CurrentPosition.X += brickResult.Difference.X + ball->Size.X / 2.f;
                currentDirection.X *= -1;
                break;
            case RIGHT:
                ball->CurrentPosition.X -= brickResult.Difference.X + ball->Size.X / 2.f;
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
        currentDirection.X *= -1;
        ENT_MoveEntity(ball, currentDirection, deltaTime);
    }

    if (ballPosition.Y <= 0)
    {
        currentDirection.Y *= -1;

        ENT_MoveEntity(ball, currentDirection, deltaTime);
    }
}

static void BallDied(void)
{
    if (!ball->IsEnabled) return;

    ball->IsEnabled = false;

    if (--lives <= 0)
    {
        GAM_SetGameWon(false);
        shouldFinishNextFrame = true;
    }
}

static void UnstickBall(void)
{
    static int stuckXFrameCount = 0;
    static float lastFrameX = 0;
    const VectorF2D ballPosition = ball->CurrentPosition;

    if (UTL_BetweenF(0, ball->Size.X, ballPosition.X)
        || UTL_BetweenF(windowWidth - ball->Size.X, windowWidth, ballPosition.X))
    {
        stuckXFrameCount++;
    }
    else
    {
        lastFrameX = ballPosition.X;
        stuckXFrameCount = 0;
    }

    if (stuckXFrameCount >= STUCK_FRAME_COUNT)
    {
        stuckXFrameCount = 0;
        VectorF2D newDirection = lastFrameX > windowWidth / 2.f ? UTL_GetLeftVectorF() : UTL_GetRightVectorF();
        if (UTL_GetRandom(0, 1))
        {
            newDirection = UTL_AddVectorF2D(newDirection, UTL_GetUpVectorF());
        }
        else newDirection = UTL_AddVectorF2D(newDirection, UTL_GetDownVectorF());

        ENT_MoveEntity(ball, newDirection, GAM_GetDeltaSeconds());
    }

    static int stuckYFrameCount = 0;
    static float lastFrameY = 0;

    if (UTL_BetweenF(lastFrameY - 100, lastFrameY + 100, ballPosition.Y))
    {
        stuckYFrameCount++;
    }
    else
    {
        lastFrameY = ballPosition.Y;
        stuckYFrameCount = 0;
    }

    if (stuckYFrameCount >= STUCK_FRAME_COUNT)
    {
        stuckYFrameCount = 0;
        VectorF2D newDirection = ballPosition.Y > windowHeight / 2.f ? UTL_GetDownVectorF() : UTL_GetUpVectorF();

        if (UTL_GetRandom(0, 1))
        {
            newDirection = UTL_AddVectorF2D(newDirection, UTL_GetRightVectorF());
        }
        else newDirection = UTL_AddVectorF2D(newDirection, UTL_GetLeftVectorF());

        ENT_MoveEntity(ball, newDirection, GAM_GetDeltaSeconds());
    }
}

static void DrawScoreAndLives(void)
{
    static int frameScore = -1;
    static int frameLives = -1;
    static char buffer[12];

    if (frameScore != GAM_GetScore())
    {
        frameScore = GAM_GetScore();
        REN_FreeTexture(scoreTexture);

        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "Score: %d", frameScore);

        scoreTexture = TXT_CreateTextureFromText(buffer);
    }

    REN_DrawTexture(scoreTexture,
        UTL_MakeVector2D(TEXT_SIDE_PADDING, TEXT_TOP_PADDING));

    if (frameLives != lives)
    {
        frameLives = lives;
        REN_FreeTexture(lifeTexture);

        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "Lives: %i", lives);

        lifeTexture = TXT_CreateTextureFromText(buffer);
    }

    REN_DrawTexture(lifeTexture,
        UTL_MakeVector2D((int)windowWidth - lifeTexture->Width - TEXT_SIDE_PADDING, TEXT_TOP_PADDING));
}
