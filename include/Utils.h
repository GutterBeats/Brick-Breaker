//
// Created by Anthony Lesch on 7/17/24.
//

#pragma once

#include "Types.h"

#pragma region Assertion Macros

#define ASSERT_NOTNULL(e, name) if (e == NULL) { \
             SDL_Log("Object (%s) was null!", name); \
             exit(1);\
}

#pragma endregion

#pragma region Math Utility Functions

/**
 * Utility function to clamp a float value between a min and max value.
 *
 * @param min The smallest allowable float value.
 * @param max The largest allowable float value.
 * @param value The current float value.
 *
 * @return A float that is clamped between min and max.
 */
static float UTL_FClamp(const float min, const float max, const float value)
{
    return value > max ? max
                       : value < min ? min : value;
}

/**
 * Utility function to clamp an integer value between a min and max value.
 *
 * @param min The smallest allowable integer value.
 * @param max The largest allowable integer value.
 * @param value The current integer value.
 *
 * @return An integer that is clamped between min and max.
 */
static int UTL_Clamp(const int min, const int max, const int value)
{
    return value > max ? max
                       : value < min ? min : value;
}

/**
 * Utility function to lerp between two values.
 *
 * @param a The current value.
 * @param b The value to lerp to.
 * @param t The interpolation speed.
 *
 * @return A float value between a and b.
 */
static float UTL_Lerp(const float a, const float b, const float t)
{
    return a + (b - a) * UTL_FClamp(0, 1, t);
}

/**
 * Checks if the value is between the min and max values.
 *
 * @param min The minimum number.
 * @param max The maximum number.
 * @param value The value to check.
 *
 * @return A boolean value indicating if the value is between the min and max values.
 **/
static bool UTL_Between(const float min, const float max, const float value)
{
    return value >= min && value <= max;
}

#pragma endregion

#pragma region Random Value Utility Functions

/**
 * Sets the seed for the srand() utility. Can only be called once per program instance.
 * Subsequent calls will be ignored.
 **/
void UTL_SetRandomSeed(void);

/**
 * Gets a pseudorandom number between the supplied upper and lower bounds.
 * Define the MAX_RAND_COUNT macro to change the number of tries the generator takes to get a random number.
 * The default is 20.
 *
 * @param lower The lower bound of the random number.
 * @param upper The upper bound of the random number.
 *
 * @return A pseudorandom integer.
 **/
int UTL_GetRandom(int lower, int upper);

#pragma endregion

#pragma region Direction Constants

static Vector2D GetUpVector(void)
{
    return (Vector2D){
        .X = 0, .Y = 1
    };
}

static Vector2D GetDownVector(void)
{
    return (Vector2D){
        .X = 0, .Y = -1
    };
}

static Vector2D GetLeftVector(void)
{
    return (Vector2D){
        .X = -1, .Y = 0
    };
}

static Vector2D GetRightVector(void)
{
    return (Vector2D){
        .X = 1, .Y = 0
    };
}

#pragma endregion