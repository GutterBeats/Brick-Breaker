//
// Created by Anthony Lesch on 7/22/24.
//

#pragma once

#include <stdbool.h>

bool InitializeText(void);
void ShutdownText(void);

void DrawText(const char* text, int x, int y);
