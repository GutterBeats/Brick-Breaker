//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include <SDL.h>

#include "Types.h"

u8 InitializeRenderer(SDL_Window* window);

void BeginDrawing(void);
void FinishDrawing(void);
void DestroyRenderer(void);

void DrawTexture(SDL_Texture* texture, const SDL_Rect* source, const SDL_Rect* dest);
void DrawTextureF(SDL_Texture* texture, const SDL_Rect* source, const SDL_FRect* dest);

SDL_Texture* LoadTexture(const char* filePath, int* textureWidth, int* textureHeight);
SDL_Texture* LoadTextureFromSurface(SDL_Surface* surface);
void FreeTexture(SDL_Texture* texture);
