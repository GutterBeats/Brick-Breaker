//
// Created by Anthony Lesch on 7/17/24.
//

#pragma once

static inline float clamp(float min, float max, float value)
{
    return value > max ? max
                       : value < min ? min : value;
}

static inline float lerp(float a, float b, float t)
{
    return a + (b - a) * clamp(0, 1, t);
}
