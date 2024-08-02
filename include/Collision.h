//
// Created by Anthony Lesch on 8/1/24.
//

#pragma once

#include <stdbool.h>

#include "Types.h"

typedef enum collision_direction
{
    DOWN,
    RIGHT,
    UP,
    LEFT
} CollisionDirection;

typedef struct collision_volume
{
    VectorF2D Position;
    VectorF2D Size;
} CollisionVolume;

typedef struct collision_result
{
    bool Collided;
    CollisionDirection Direction;
    VectorF2D Difference;
} CollisionResult;

CollisionVolume* COL_MakeCollisionVolume(VectorF2D position, VectorF2D size);

CollisionResult COL_HasCollisionBoxCircle(const CollisionVolume* box, const CollisionVolume* circle);
CollisionDirection COL_GetCollisionDirection(VectorF2D vector);
