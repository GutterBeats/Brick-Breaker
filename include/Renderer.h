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

void REN_BeginDrawing(void);
void REN_FinishDrawing(void);
void REN_DestroyRenderer(void);

void REN_DrawTexture(const Texture* texture, Vector2D position);
void REN_DrawTexture_Alpha(const Texture* texture, Vector2D position, float alpha);

void REN_DrawRectangleF(VectorF2D position, VectorF2D size);
void REN_DrawTextureF(const Texture* texture, VectorF2D position);

Texture* REN_LoadTexture(const char* filePath);
Texture* REN_LoadTextureFromSurface(SDL_Surface* surface);
void REN_FreeTexture(Texture* texture);
