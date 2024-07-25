//
// Created by Anthony Lesch on 7/21/24.
//

#include "BrickManager.h"

#include "Game.h"
#include "Renderer.h"
#include "Utils.h"
#include "Resources.h"

#define BRICK_PADDING 15

static Brick* CreateBrick(float x, float y, int health);
static const char* GetTextureForHealth(int health);

BrickManager* CreateBricks(float x, float y, const float w, const float h)
{
    int brickWidth, brickHeight;
    SDL_Texture* brickTexture = LoadTexture(GREEN_BRICK_TEXTURE, &brickWidth, &brickHeight);
    if (brickTexture == NULL)
    {
        brickWidth = 64;
        brickHeight = 32;

        SDL_Log(
            "Unable to load brick width and height from texture. Using default size: w=%d, h=%d",
            brickWidth, brickHeight);
    }

    FreeTexture(brickTexture);

    const int cols = w / (brickWidth + BRICK_PADDING);
    const int rows = h / (brickHeight + BRICK_PADDING);

    BrickManager* manager = malloc(sizeof(BrickManager));
    if (manager == NULL) return NULL;

    manager->Bricks = calloc(rows * cols, sizeof(Brick*));
    if (manager->Bricks == NULL)
    {
        free(manager);
        return NULL;
    }

    x -= BRICK_PADDING;
    y -= BRICK_PADDING;

    manager->BrickCount = 0;

    for (int r = 0; r < rows; ++r)
    {
        const int rowHealth = UTL_GetRandom(
            15 * (rows - r) * 0.5,
            30 * (rows - r) * 0.5);

        for (int c = 0; c < cols; ++c)
        {
            Brick* brick = CreateBrick(
                    x + (brickWidth + BRICK_PADDING) * c,
                    y + (brickHeight + BRICK_PADDING) * r,
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
        const Brick* current = manager->Bricks[i];
        if (!current->IsEnabled) continue;

        DrawTextureF(current->Texture, NULL, &current->Bounds);
    }
}

void CheckBrickCollision(const BrickManager* manager, float otherX, float otherY, Brick* collision)
{
    for (size_t i = 0; i < manager->BrickCount; ++i)
    {
        const Brick* current = manager->Bricks[i];
        if (!current->IsEnabled) continue;


    }
}

void DestroyManager(BrickManager* manager)
{
    if (manager == NULL) return;

    for (int i = 0; i < manager->BrickCount; ++i)
    {
        // ReSharper disable once CppDFANullDereference
        Brick* brick = manager->Bricks[i];
        if (brick == NULL) break;

        FreeTexture(brick->Texture);
        free(brick);
    }

    free(manager);
}

static Brick* CreateBrick(const float x, const float y, const int health)
{
    Brick* brick = malloc(sizeof(Brick));
    if (brick == NULL) return NULL;

    const char* texture = GetTextureForHealth(health);
    int width, height;

    brick->Texture = LoadTexture(texture, &width, &height);
    if (brick->Texture == NULL)
    {
        free(brick);
        return NULL;
    }

    brick->Bounds = (SDL_FRect){
        .x = x,
        .y = y,
        .w = (float)width,
        .h = (float)height
    };
    brick->Health = health;
    brick->IsEnabled = true;

    return brick;
}

static const char* GetTextureForHealth(int health)
{
    return GREEN_BRICK_TEXTURE;
}
