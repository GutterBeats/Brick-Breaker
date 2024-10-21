//
// Created by Anthony Lesch on 7/21/24.
//

#include "BrickManager.h"

#include "Collision.h"
#include "Game.h"
#include "Renderer.h"
#include "Resources.h"
#include "Utilities.h"

#define MIN_ROW_COUNT 3
#define MAX_ROW_COUNT 10

static Entity* CreateBrick(float, float, int);
static const char* GetTexturePathForHealth(int);

// IMPR: Refactor this to load level layouts from a file. Or center it in the window.
BrickManager* BM_CreateBricks(VectorF2D startPosition, const VectorF2D containerSize, const float padding)
{
    int brickWidth, brickHeight;

    // Making the assumption that all brick textures are the same size. And they are in this instance.
    Texture* brickTexture = REN_LoadTexture(GREEN_BRICK_TEXTURE);
    if (brickTexture != NULL)
    {
        brickWidth = brickTexture->Width;
        brickHeight = brickTexture->Height;
    }
    else
    {
        brickWidth = 64;
        brickHeight = 32;

        BB_LOG(
            "Unable to load brick width and height from texture. Using default size: w=%d, h=%d",
            brickWidth, brickHeight);
    }

    const int cols = containerSize.X / (brickWidth + padding);
    const int rows = UTL_Clamp(MIN_ROW_COUNT, MAX_ROW_COUNT, containerSize.Y / (brickHeight + padding));

    REN_FreeTexture(brickTexture);

    BrickManager* manager = malloc(sizeof(BrickManager));
    if (manager == NULL) return NULL;

    manager->Bricks = calloc(rows * cols, sizeof(Entity*));
    if (manager->Bricks == NULL)
    {
        free(manager);
        return NULL;
    }

    startPosition.X -= padding;
    startPosition.Y -= padding;

    manager->BrickCount = 0;

    for (int r = 0; r < rows; ++r)
    {
        const int rowHealth = UTL_GetRandom(
            15 * (rows - r) * 0.5,
            30 * (rows - r) * 0.5);

        for (int c = 0; c < cols; ++c)
        {
            Entity* brick = CreateBrick(
                    startPosition.X + (brickWidth + padding) * c,
                    startPosition.Y + (brickHeight + padding) * r,
                    rowHealth);

            if (brick == NULL)
            {
                BM_DestroyManager(manager);
                free(brick);

                return NULL;
            }

            char buffer[20];
            sprintf(buffer, "Brick [%i, %i]", r, c);

            brick->Name = strdup(buffer);
            manager->Bricks[manager->BrickCount++] = brick;
        }
    }

    manager->Position = startPosition;
    manager->Size = containerSize;
    manager->EnabledBrickCount = manager->BrickCount;

    return manager;
}

void BM_DrawBricks(const BrickManager* manager)
{
    for (size_t i = 0; i < manager->BrickCount; ++i)
    {
        const Entity* current = manager->Bricks[i];
        if (!current->IsEnabled) continue;

        REN_DrawTextureF(current->Texture, current->CurrentPosition);
    }
}

CollisionResult BM_CheckBrickCollision(BrickManager* manager, const Entity* ball)
{
    for (size_t i = 0; i < manager->BrickCount; ++i)
    {
        Entity* current = manager->Bricks[i];
        if (!current->IsEnabled)
            continue;

        const CollisionResult result = COL_HasCollisionBoxCircle(current->CollisionVolume, ball->CollisionVolume);
        if (result.Collided)
        {
            GAM_UpdateScore(UTL_Min(current->Health, ball->DamageGiven));

            current->Health -= ball->DamageGiven;
            if (current->Health <= 0)
            {
                current->IsEnabled = false;
                manager->EnabledBrickCount -= 1;

                BB_LOG("Brick Count: %i", manager->EnabledBrickCount);
            }

            return result;
        }
    }

    return (CollisionResult) { .Collided = false };
}

void BM_DestroyManager(BrickManager* manager)
{
    if (manager == NULL) return;

    for (int i = 0; i < manager->BrickCount; ++i)
    {
        // ReSharper disable once CppDFANullDereference
        ENT_DestroyEntity(manager->Bricks[i]);
    }

    free(manager->Bricks);
    free(manager);
}

static Entity* CreateBrick(const float x, const float y, const int health)
{
    const char* texturePath = GetTexturePathForHealth(health);

    Entity* brick = ENT_CreateEntity(
        UTL_MakeVectorF2D(x, y), texturePath);

    if (brick == NULL) return NULL;

    brick->Health = health;
    brick->IsEnabled = true;
    brick->CollisionVolume = COL_MakeCollisionVolume(brick->CurrentPosition, brick->Size);

    return brick;
}

static const char* GetTexturePathForHealth(const int health)
{
    if (health >= 80)
    {
        return RED_BRICK_TEXTURE;
    }

    if (health >= 60)
    {
        return GREY_BRICK_TEXTURE;
    }

    if (health >= 40)
    {
        return PURPLE_BRICK_TEXTURE;
    }

    if (health >= 20)
    {
        return BLUE_BRICK_TEXTURE;
    }

    return GREEN_BRICK_TEXTURE;
}
