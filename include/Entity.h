//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include <SDL.h>
#include "Types.h"

typedef struct entity
{
    VectorF2D Position;
    VectorF2D Size;

    SDL_Texture* Texture;
} Entity;

Entity* CreateEntity(VectorF2D position, VectorF2D size, const char* texturePath);
void DestroyEntity(Entity* entity);

SDL_FRect GetEntityRectangle(Entity* entity);
