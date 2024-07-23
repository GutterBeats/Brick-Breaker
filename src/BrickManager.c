//
// Created by Anthony Lesch on 7/21/24.
//

#include "BrickManager.h"

#include "Game.h"
#include "Utils.h"

#define BRICK_TEXTURE "resources/Textures/element_blue_rectangle_glossy.png"
#define BRICK_PADDING 15

static Brick* CreateBrick(float x, float y, int health, const char* texture);

Brick** CreateBricks(float x, float y, const float w, const float h, size_t* count)
{
    Entity* initial = CreateEntity(x, y, BRICK_TEXTURE);
    if (initial == NULL) return NULL;

    const float brickWidth = initial->Bounds.w;
    const float brickHeight = initial->Bounds.h;

    DestroyEntity(initial);

    const int cols = w / (brickWidth + BRICK_PADDING);
    const int rows = h / (brickHeight + BRICK_PADDING);

    Brick** bricks = calloc(rows * cols, sizeof(Brick*));
    if (bricks == NULL) return NULL;

    int i = 0;
    x -= BRICK_PADDING;
    y -= BRICK_PADDING;

    for (int r = 0; r < rows; ++r)
    {
        const int rowHealth = UTL_GetRandom(
            15 * (rows - r) * 0.5,
            30 * (rows - r) * 0.5);

        SDL_Log("Row %d Health: %d", r, rowHealth);

        for (int c = 0; c < cols; ++c)
        {
            const float xPos = x + (brickWidth + BRICK_PADDING) * c;
            const float yPos = y + (brickHeight + BRICK_PADDING) * r;

            Brick* brick = CreateBrick(xPos, yPos, rowHealth, BRICK_TEXTURE);

            if (brick == NULL)
            {
                free(bricks);
                free(brick);

                return NULL;
            }

            bricks[i++] = brick;
        }
    }

    if (count != NULL)
    {
        *count = i;
    }

    return bricks;
}

void DestroyBricks(Brick** bricks, const size_t brickCount)
{
    for (int i = 0; i < brickCount; ++i)
    {
        DestroyEntity(bricks[i]->Entity);
        free(bricks[i]);
    }
}

static Brick* CreateBrick(const float x, const float y, const int health, const char* texture)
{
    Brick* brick = malloc(sizeof(Brick));
    if (brick == NULL)
    {
        return NULL;
    }

    brick->Entity = CreateEntity(x, y, texture);
    if (brick->Entity == NULL)
    {
        free(brick);
        return NULL;
    }

    brick->Health = health;
    brick->Entity->IsEnabled = true;

    return brick;
}

void CheckBrickCollision(Brick** bricks, const size_t brickCount, const float otherX, const float otherY, Brick* collision)
{
    if (brickCount == 0) return;

    for (size_t i = 0; i < brickCount; ++i)
    {
        const Brick* current = bricks[i];
        if (!current->Entity->IsEnabled) continue;

        
    }
}
