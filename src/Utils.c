//
// Created by Anthony Lesch on 7/17/24.
//

#include <stdlib.h>
#include <time.h>

#include "Utils.h"

#ifndef MAX_RAND_COUNT
  #define MAX_RAND_COUNT 20
#endif

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
