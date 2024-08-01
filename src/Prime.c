//
// Created by Anthony Lesch on 7/31/24.
//

#include <math.h>
#include "Prime.h"

int IsPrime(const int x)
{
    if (x < 2) return -1;
    if (x < 4) return 1;
    if (x % 2 == 0) return 0;

    for (int i = 3; i <= floor(sqrt(x)); i += 2)
    {
        if (x % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

int NextPrime(int x)
{
    while (IsPrime(x) != 1)
    {
        x++;
    }

    return x;
}
