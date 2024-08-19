//
// Created by Anthony Lesch on 7/15/24.
//

#pragma once

#include <stdint.h>
#include <SDL.h>

//----------------------------------------------------------------------------------
// Types and Structures Definitions
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

typedef struct scene_t
{
    void (*Initialize)(void);
    void (*Update)(float deltaTime);
    void (*Draw)(void);
    void (*Destroy)(void);
} Scene;

typedef struct vector2D
{
    int X;
    int Y;
} Vector2D;

typedef struct vectorF2d
{
    float X;
    float Y;
} VectorF2D;

//----------------------------------------------------------------------------------
// Scene Extern Variables
//----------------------------------------------------------------------------------
extern Scene TitleScene;
extern Scene MenuScene;
extern Scene OptionScene;
extern Scene GameplayScene;
extern Scene EndingScene;
