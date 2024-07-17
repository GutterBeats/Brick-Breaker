//
// Created by Anthony Lesch on 7/15/24.
//

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <SDL_rect.h>

//----------------------------------------------------------------------------------
// Type Definitions
//----------------------------------------------------------------------------------
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen
{
    UNKNOWN = -1,
    TITLE = 0,
    OPTIONS,
    GAMEPLAY,
    ENDING
} GameScreen;

typedef struct VectorF2D
{
    f32 X;
    f32 Y;
} VectorF2D;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
