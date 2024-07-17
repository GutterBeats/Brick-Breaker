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

SDL_Texture* LoadTexture(const char* filePath);
