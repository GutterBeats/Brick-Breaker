//
// Created by Anthony Lesch on 8/1/24.
//

#pragma once

#include "Types.h"

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

BoxCollision* COL_MakeBoxCollisionVolume(VectorF2D position, VectorF2D size);
CircleCollision* COL_MakeCircleCollisionVolume(VectorF2D position, float radius);

bool COL_HasCollisionBox(const BoxCollision* first, const BoxCollision* second);
bool COL_HasCollisionBoxCircle(const BoxCollision* first, const CircleCollision* second);
