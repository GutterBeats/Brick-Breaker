//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include <SDL.h>

#include "Types.h"

typedef struct texture
{
    int Width;
    int Height;

    SDL_Texture* Data;
} Texture;

u8 InitializeRenderer(SDL_Window* window);

void BeginDrawing(void);
void FinishDrawing(void);
void DestroyRenderer(void);

void DrawTexture(const Texture* texture, int x, int y);
void DrawTexture_Alpha(const Texture* texture, int x, int y, float alpha);

void DrawTextureF(const Texture* texture, float x, float y);
void DrawTextureF_Alpha(const Texture* texture, float x, float y, float alpha);

Texture* LoadTexture(const char* filePath);
Texture* LoadTextureFromSurface(SDL_Surface* surface);
void FreeTexture(Texture* texture);
