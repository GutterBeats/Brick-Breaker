//
// Created by Anthony Lesch on 7/22/24.
//

#pragma once

#include "Types.h"
#include "Renderer.h"

void TXT_InitializeText(void);
void TXT_DestroyTextSystem(void);

void TXT_DrawText(const char* text, Vector2D position);

Texture* TXT_CreateTextureFromText(const char* text);
Texture* TXT_CreateTextureFromText_Large(const char* text);
