//
// Created by Anthony Lesch on 7/20/24.
//

#pragma once

#include "Types.h"
#include "stdbool.h"

typedef struct game_t
{
    int ScreenWidth;
    int ScreenHeight;
    int Score;

    float TimeScale;
    float DeltaSeconds;
    float FPS;

    bool GameWon;
    bool IsRunning;
    bool IsPaused;
    bool ShowDebug;

    Scene** SceneLayers;
} Game;

void GAM_InitializeGameSystems(const char* title, int desiredScreenWidth, int desiredScreenHeight);
void GAM_ShutdownGameSystems(void);
void GAM_QuitGame(void);
void GAM_TransitionToScene(Scene* newScene);

void GAM_PushSceneLayer(Scene* layer);
void GAM_PopSceneLayer(void);

void GAM_GetScreenDimensions(int* width, int* height);
void GAM_UpdateScore(int points);
int GAM_GetScore(void);

bool GAM_GetGameWon(void);
void GAM_SetGameWon(bool value);
bool GAM_GetIsGameRunning(void);
void GAM_SetIsGameRunning(bool running);

bool GAM_GetIsPaused(void);
void GAM_SetIsPaused(bool paused);

bool GAM_GetShowDebug(void);
void GAM_SetShowDebug(bool showDebug);

void GAM_StartFrame(void);
void GAM_UpdateCurrentScene(void);
void GAM_EndFrame(void);
void GAM_SetTimeScale(float scale);
void GAM_SetTimeScaleForSeconds(float scale, float seconds);
float GAM_GetDeltaSeconds(void);
float GAM_GetFPS(void);
void GAM_DrawFPS(Vector2D position);
