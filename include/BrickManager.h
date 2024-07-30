//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include "Entity.h"

// IMPR: Use flywheel pattern to reuse texture for bricks that share texture.
typedef struct brick_manager
{
    int BrickCount;

    Entity** Bricks;
} BrickManager;

BrickManager* CreateBricks(VectorF2D startPosition, VectorF2D containerSize, float padding);
void DrawBricks(const BrickManager* manager);

bool CheckBrickCollision(const BrickManager* manager, Entity* ball, size_t* collisionIndex);

void DestroyManager(BrickManager* manager);
