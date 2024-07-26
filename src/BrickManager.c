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

static Entity* CreateBrick(float x, float y, int health);
static const char* GetTextureForHealth(int health);

// TODO: Refactor this to load level layouts from a file. Or center it in the window.
BrickManager* CreateBricks(float x, float y, const float w, const float h, const float padding)
{
    int brickWidth, brickHeight;

    // Making the assumption that all brick textures are the same size. And they are in this instance.
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

    const int cols = w / (brickWidth + padding);
    const int rows = UTL_Clamp(MIN_ROW_COUNT, MAX_ROW_COUNT, h / (brickHeight + padding));

    BrickManager* manager = malloc(sizeof(BrickManager));
    if (manager == NULL) return NULL;

    manager->Bricks = calloc(rows * cols, sizeof(Entity*));
    if (manager->Bricks == NULL)
    {
        free(manager);
        return NULL;
    }

    x -= padding;
    y -= padding;

    manager->BrickCount = 0;

    for (int r = 0; r < rows; ++r)
    {
        const int rowHealth = UTL_GetRandom(
            15 * (rows - r) * 0.5,
            30 * (rows - r) * 0.5);

        for (int c = 0; c < cols; ++c)
        {
            Entity* brick = CreateBrick(
                    x + (brickWidth + padding) * c,
                    y + (brickHeight + padding) * r,
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

        DrawTextureF(current->Texture, NULL, &current->Bounds);
    }
}

bool CheckBrickCollision(const BrickManager* manager, const Entity* other)
{
    for (size_t i = 0; i < manager->BrickCount; ++i)
    {
        const Entity* current = manager->Bricks[i];
        if (!current->IsEnabled) continue;

        const SDL_FRect bounds = current->Bounds;

        if (UTL_Between(bounds.x, bounds.x + bounds.w, other->Bounds.x)
            && UTL_Between(bounds.y, bounds.y + bounds.h, other->Bounds.y))
        {
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
