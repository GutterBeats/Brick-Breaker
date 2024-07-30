//
// Created by Anthony Lesch on 7/21/24.
//

#include "BrickManager.h"
#include "Game.h"
#include "Renderer.h"
#include "Utils.h"
#include "Resources.h"

#define MIN_ROW_COUNT 3
#define MAX_ROW_COUNT 10

static Entity* CreateBrick(float, float, int);
static const char* GetTexturePathForHealth(int);

// TODO: Refactor this to load level layouts from a file. Or center it in the window.
BrickManager* CreateBricks(VectorF2D startPosition, const VectorF2D containerSize, const float padding)
{
    int brickWidth, brickHeight;

    // Making the assumption that all brick textures are the same size. And they are in this instance.
    Texture* brickTexture = LoadTexture(GREEN_BRICK_TEXTURE);
    if (brickTexture != NULL)
    {
        brickWidth = brickTexture->Width;
        brickHeight = brickTexture->Height;
    }
    else
    {
        brickWidth = 64;
        brickHeight = 32;

        SDL_Log(
            "Unable to load brick width and height from texture. Using default size: w=%d, h=%d",
            brickWidth, brickHeight);
    }

    const int cols = containerSize.X / (brickWidth + padding);
    const int rows = UTL_Clamp(MIN_ROW_COUNT, MAX_ROW_COUNT, containerSize.Y / (brickHeight + padding));

    FreeTexture(brickTexture);

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
                DestroyManager(manager);
                free(brick);

                return NULL;
            }

            manager->Bricks[manager->BrickCount++] = brick;
        }
    }

    return manager;
}

void DrawBricks(const BrickManager* manager)
{
    for (size_t i = 0; i < manager->BrickCount; ++i)
    {
        const Entity* current = manager->Bricks[i];
        if (!current->IsEnabled) continue;

        DrawTextureF(current->Texture, current->Position);
    }
}

bool CheckBrickCollision(const BrickManager* manager, Entity* ball, size_t* collisionIndex)
{
    for (size_t i = 0; i < manager->BrickCount; ++i)
    {
        Entity* current = manager->Bricks[i];
        if (!current->IsEnabled) continue;

        if (ENT_HasCollision(ball, current))
        {
            current->Health -= ball->DamageGiven;
            if (current->Health <= 0)
            {
                current->IsEnabled = false;
            }

            *collisionIndex = i;
            return true;
        }
    }

    return false;
}

void DestroyManager(BrickManager* manager)
{
    if (manager == NULL) return;

    for (int i = 0; i < manager->BrickCount; ++i)
    {
        // ReSharper disable once CppDFANullDereference
        Entity* brick = manager->Bricks[i];
        if (brick == NULL) break;

        FreeTexture(brick->Texture);
        free(brick);
    }

    free(manager);
}

static Entity* CreateBrick(const float x, const float y, const int health)
{
    Entity* brick = malloc(sizeof(Entity));
    if (brick == NULL) return NULL;

    brick->Texture = LoadTexture(
        GetTexturePathForHealth(health));

    if (brick->Texture == NULL)
    {
        free(brick);
        return NULL;
    }

    brick->CurrentPosition = (VectorF2D){ x, y };
    brick->PreviousPosition = (VectorF2D){0,0};
    brick->Size = (VectorF2D){ brick->Texture->Width, brick->Texture->Height };

    brick->Health = health;
    brick->IsEnabled = true;

    return brick;
}

static const char* GetTexturePathForHealth(int health)
{
    return GREEN_BRICK_TEXTURE;
}
