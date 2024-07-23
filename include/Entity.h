//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include <SDL.h>
#include "Types.h"

typedef struct entity
{
    SDL_FRect Bounds;

    SDL_Texture* Texture;

    bool IsEnabled;
} Entity;

Entity* CreateEntity(float x, float y, const char* texturePath);

void DrawEntity(const Entity* entity, const SDL_Rect* source);
void DestroyEntity(Entity* entity);

bool HasTopCollision(const Entity* first, const Entity* second);
