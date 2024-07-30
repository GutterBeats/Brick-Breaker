//
// Created by Anthony Lesch on 7/17/24.
//

#include <stdlib.h>
#include <time.h>

#include "Utils.h"

#ifndef MAX_RAND_COUNT
  #define MAX_RAND_COUNT 20
#endif

Vector2D UTL_MakeVector2D(int x, int y)
{
    return (Vector2D){ x, y };
}

Vector2D UTL_ScaleVector2D(const Vector2D vector, int factor)
{
    return (Vector2D){
        vector.X * factor,
        vector.Y * factor
    };
}

VectorF2D UTL_MakeVectorF2D(float x, float y)
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
