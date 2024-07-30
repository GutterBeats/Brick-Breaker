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

void DrawRectangle(Vector2D position, Vector2D size);
void DrawTexture(const Texture* texture, Vector2D position);
void DrawTexture_Alpha(const Texture* texture, Vector2D position, float alpha);

void DrawRectangleF(VectorF2D position, VectorF2D size);
void DrawTextureF(const Texture* texture, VectorF2D position);
void DrawTextureF_Alpha(const Texture* texture, VectorF2D position, float alpha);

Texture* LoadTexture(const char* filePath);
Texture* LoadTextureFromSurface(SDL_Surface* surface);
void FreeTexture(Texture* texture);
