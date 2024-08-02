//
// Created by Anthony Lesch on 8/1/24.
//

#include "CollisionVolume.h"
#include "Utils.h"

BoxCollision* COL_MakeBoxCollisionVolume(VectorF2D position, VectorF2D size)
{
    BoxCollision* collision = malloc(sizeof(BoxCollision));
    if (collision == NULL)
    {
        SDL_Log("Unable to allocate memory for Circle Collision volume.");
        return NULL;
    }

    collision->Position = position;
    collision->Size = size;

    return collision;
}

CircleCollision* COL_MakeCircleCollisionVolume(const VectorF2D position, const float radius)
{
    CircleCollision* collision = malloc(sizeof(CircleCollision));
    if (collision == NULL)
    {
        SDL_Log("Unable to allocate memory for Circle Collision volume.");
        return NULL;
    }

    collision->Position = position;
    collision->Radius = radius;

    return collision;
}

bool COL_HasCollisionBox(const BoxCollision* first, const BoxCollision* second)
{
    // collision x-axis?
    const bool collisionX = first->Position.X + first->Size.X >= second->Position.X
        && second->Position.X + second->Size.X >= first->Position.X;

    // collision y-axis?
    const bool collisionY = first->Position.Y + first->Size.Y >= second->Position.Y
        && second->Position.Y + second->Size.Y >= first->Position.Y;

    // collision only if on both axes
    return collisionX && collisionY;
}

bool COL_HasCollisionBoxCircle(const BoxCollision* first, const CircleCollision* second)
{
    const VectorF2D halfBox = UTL_MakeVectorF2D(first->Size.X / 2.f, first->Size.Y / 2.f);
    const VectorF2D boxCenter = UTL_MakeVectorF2D(
        first->Position.X + halfBox.X, first->Position.Y + halfBox.Y);
    const VectorF2D circleCenter = UTL_MakeVectorF2D(
        second->Position.X + second->Radius, second->Position.Y + second->Radius);

    VectorF2D difference = UTL_SubtractVectorF2D(second->Position, boxCenter);
    const VectorF2D clamped = UTL_MakeVectorF2D(UTL_FClamp(-halfBox.X, halfBox.X, difference.X),
                                                UTL_FClamp(-halfBox.Y, halfBox.Y, difference.Y));

    const VectorF2D closest = UTL_AddVectorF2D(clamped, boxCenter);
    difference = UTL_SubtractVectorF2D(closest, circleCenter);

    return UTL_GetVectorF2DLength(difference) < second->Radius;
}
