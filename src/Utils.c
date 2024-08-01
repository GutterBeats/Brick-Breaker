//
// Created by Anthony Lesch on 7/17/24.
//

#include <stdlib.h>
#include <time.h>

#include "Utils.h"

#ifndef MAX_RAND_COUNT
  #define MAX_RAND_COUNT 20
#endif

Vector2D UTL_AddVector2D(const Vector2D left, const Vector2D right)
{
    return UTL_MakeVector2D(left.X + right.X, left.Y + right.Y);
}

Vector2D UTL_SubtractVector2D(const Vector2D left, const Vector2D right)
{
    return UTL_MakeVector2D(left.X - right.X, left.Y - right.Y);
}

Vector2D UTL_MakeVector2D(const int x, const int y)
{
    return (Vector2D){ x, y };
}

Vector2D UTL_ScaleVector2D(const Vector2D vector, const int factor)
{
    return (Vector2D){
        vector.X * factor,
        vector.Y * factor
    };
}

Vector2D UTL_NormalizeVector2D(Vector2D vector)
{
    const int magnitude = sqrt(vector.X * vector.X + vector.Y * vector.Y);
    if (magnitude == 0) return vector;

    return UTL_MakeVector2D(vector.X / magnitude, vector.Y / magnitude);
}

VectorF2D UTL_AddVectorF2D(const VectorF2D left, const VectorF2D right)
{
    return UTL_MakeVectorF2D(left.X + right.X, left.Y + right.Y);
}

VectorF2D UTL_SubtractVectorF2D(const VectorF2D left, const VectorF2D right)
{
    return UTL_MakeVectorF2D(left.X - right.X, left.Y - right.Y);
}

VectorF2D UTL_MakeVectorF2D(const float x, const float y)
{
    return (VectorF2D){ x, y };
}

VectorF2D UTL_ScaleVectorF2D(const VectorF2D vector, const float factor)
{
    return (VectorF2D){
        vector.X * factor,
        vector.Y * factor
    };
}

VectorF2D UTL_NormalizeVectorF2D(VectorF2D vector)
{
    const float magnitude = sqrt(vector.X * vector.X + vector.Y * vector.Y);
    if (magnitude == 0) return vector;

    return UTL_MakeVectorF2D(vector.X / magnitude, vector.Y / magnitude);
}

void UTL_SetRandomSeed(void)
{
    static bool seeded = false;
    if (seeded) return;

    seeded = true;
    srand(time(NULL));
}

int UTL_GetRandom(const int lower, const int upper)
{
    for (int i = 0; i < MAX_RAND_COUNT; ++i)
    {
        const int r = UTL_Clamp(lower, RAND_MAX, rand() % upper);
        if (r >= lower)
        {
            return r;
        }
    }

    return upper;
}

void UTL_PrintVector(const Vector2D vector)
{
    SDL_Log("VectorF2D: X=%d, Y=%d", vector.X, vector.Y);
}

void UTL_PrintVectorF(const VectorF2D vector)
{
    SDL_Log("VectorF2D: X=%6.2f, Y=%6.2f", vector.X, vector.Y);
}
