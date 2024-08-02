//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include "Collision.h"
#include "Entity.h"

// IMPR: Use flywheel pattern to reuse texture for bricks that share texture.
typedef struct brick_manager
{
    int BrickCount;

    Entity** Bricks;
} BrickManager;

BrickManager* BM_CreateBricks(VectorF2D startPosition, VectorF2D containerSize, float padding);
void BM_DrawBricks(const BrickManager* manager);

CollisionResult BM_CheckBrickCollision(const BrickManager* manager, Entity* ball);

void BM_DestroyManager(BrickManager* manager);
