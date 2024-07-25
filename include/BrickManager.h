//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include "Entity.h"

// IMPR: Use flywheel pattern to reuse texture for bricks that share texture.
typedef struct brick
{
    int Health;

    SDL_FRect Bounds;
    SDL_Texture* Texture;

    bool IsEnabled;
} Brick;

typedef struct brick_manager
{
    int BrickCount;

    Brick** Bricks;
} BrickManager;

BrickManager* CreateBricks(float x, float y, float w, float h);
void DrawBricks(const BrickManager* manager);
void CheckBrickCollision(const BrickManager* manager, float otherX, float otherY, Brick* collision);

void DestroyManager(BrickManager* manager);
