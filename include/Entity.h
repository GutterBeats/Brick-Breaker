//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include <SDL.h>

#include "Renderer.h"

typedef struct entity
{
    int Health;
    int DamageGiven;

    SDL_FRect Bounds;

    Texture* Texture;

    bool IsEnabled;
} Entity;

Entity* CreateEntity(float x, float y, const char* texturePath);

void DrawEntity(const Entity* entity);
void DestroyEntity(Entity* entity);

bool HasTopCollision(const Entity* first, const Entity* second);
bool IsColliding(const Entity* first, const Entity* second);
