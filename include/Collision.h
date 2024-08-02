//
// Created by Anthony Lesch on 8/1/24.
//

#pragma once

#include "Types.h"

typedef enum collision_direction
{
    DOWN,
    RIGHT,
    UP,
    LEFT,
    NONE
} CollisionDirection;

typedef struct box_collision
{
    VectorF2D Position;
    VectorF2D Size;
} BoxCollision;

typedef struct circle_collision
{
    VectorF2D Position;
    float Radius;
} CircleCollision;

typedef struct collision_result
{
    bool Collided;
    CollisionDirection Direction;
    VectorF2D Difference;
} CollisionResult;

BoxCollision* COL_MakeBoxCollisionVolume(VectorF2D position, VectorF2D size);
CircleCollision* COL_MakeCircleCollisionVolume(VectorF2D position, float radius);

CollisionResult COL_HasCollisionBoxCircle(const BoxCollision* first, const CircleCollision* second);
CollisionDirection COL_GetCollisionDirection(VectorF2D vector);
