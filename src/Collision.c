//
// Created by Anthony Lesch on 8/1/24.
//

#include "Collision.h"
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

CollisionResult COL_HasCollisionBoxCircle(const BoxCollision* first, const CircleCollision* second)
{
    // get center point circle first
    const VectorF2D center = UTL_MakeVectorF2D(
        second->Position.X + second->Radius, second->Position.Y + second->Radius);

    // calculate AABB info (center, half-extents)
    const VectorF2D halfBox = UTL_MakeVectorF2D(first->Size.X / 2.0f, first->Size.Y / 2.0f);
    const VectorF2D boxCenter = UTL_MakeVectorF2D(
        first->Position.X + halfBox.X, first->Position.Y + halfBox.Y);

    // get difference vector between both centers
    VectorF2D difference = UTL_SubtractVectorF2D(center, boxCenter);
    const VectorF2D clamped = UTL_MakeVectorF2D(UTL_FClamp(-halfBox.X, halfBox.X, difference.X),
                                                UTL_FClamp(-halfBox.Y, halfBox.Y, difference.Y));

    // add clamped value to AABB_center and we get the value of box closest to circle
    const VectorF2D closest = UTL_AddVectorF2D(clamped, boxCenter);

    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = UTL_SubtractVectorF2D(closest, center);

    return (CollisionResult) {
        .Collided = UTL_GetVectorF2DLength(difference) < second->Radius,
        .Direction = COL_GetCollisionDirection(difference),
        .Difference = difference
    };
}

CollisionDirection COL_GetCollisionDirection(const VectorF2D vector)
{
    static VectorF2D compass[] =
    {
        (VectorF2D){ 0, 1 }, // Down
        (VectorF2D){ 1, 0 }, // Right
        (VectorF2D){ 0, -1 }, // Up
        (VectorF2D){ -1, 0 } // Left
    };

    float max = 0.0f;
    u32 best_match = -1;

    for (u32 i = 0; i < 4; i++)
    {
        const float dot_product = UTL_GetVectorF2DDotProduct(UTL_NormalizeVectorF2D(vector), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }

    return best_match;
}
