//
// Created by Anthony Lesch on 7/20/24.
//

#pragma once

#include "stdbool.h"

typedef struct game_t
{
    int ScreenWidth;
    int ScreenHeight;

    float TimeScale;
    float DeltaSeconds;
    float FPS;

    bool IsRunning;
    bool IsPaused;
} Game;

void InitializeGameSystems(const char* title, int desiredScreenWidth, int desiredScreenHeight);
void ShutdownGameSystems(void);

void GetScreenDimensions(int* width, int* height);

bool GetIsGameRunning(void);
void SetIsGameRunning(bool running);

bool GetIsPaused(void);
void PauseGame(void);
void UnpauseGame(void);

void StartFrame();
void EndFrame();
void SetTimeScale(float scale);
void SetTimeScaleForSeconds(float scale, float seconds);
float GetDeltaSeconds();
float GetFPS();
void DrawFPS(const int x, const int y);
