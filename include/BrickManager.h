//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include "Entity.h"

// TODO: Use flywheel pattern to reuse texture for bricks that share texture.
typedef struct brick
{
    int Health;

    Entity* Entity;
} Brick;

Brick** CreateBricks(float x, float y, float w, float h, size_t* count);

void DestroyBricks(Brick** bricks, size_t brickCount);

void CheckBrickCollision(Brick** bricks, size_t brickCount, float otherX, float otherY, Brick* collision);
