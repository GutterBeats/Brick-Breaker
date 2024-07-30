//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include "Renderer.h"

typedef struct entity
{
    int Health;
    int DamageGiven;

    VectorF2D Position;
    VectorF2D CurrentOverlap;
    VectorF2D PreviousOverlap;
    VectorF2D Size;
    VectorF2D HalfSize;

    Texture* Texture;

    bool IsEnabled;
} Entity;

Entity* ENT_CreateEntity(VectorF2D startPosition, const char* texturePath);

void ENT_DrawEntity(const Entity* entity);
void ENT_DestroyEntity(Entity* entity);

bool ENT_HasCollision(Entity* first, Entity* second);
void ENT_ResolveCollision(Entity* collider, const Entity* collidee);
