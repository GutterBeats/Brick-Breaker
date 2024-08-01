//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include "Renderer.h"

typedef struct entity
{
    int Health;
    int DamageGiven;
    const char* Name;

    float Speed;

    VectorF2D CurrentPosition;
    VectorF2D PreviousPosition;
    VectorF2D CurrentOverlap;
    VectorF2D PreviousOverlap;
    VectorF2D Size;
    VectorF2D HalfSize;

    Texture* Texture;

    bool IsEnabled;
} Entity;

Entity* ENT_CreateEntity(VectorF2D startPosition, const char* texturePath);
Entity* ENT_CreateInvisibleEntity(VectorF2D startPosition, VectorF2D size);

void ENT_MoveEntity(Entity* entity, VectorF2D direction, float deltaTime);
void ENT_DrawEntity(const Entity* entity);
void ENT_DestroyEntity(Entity* entity);
VectorF2D ENT_GetDirection(const Entity* entity);

bool ENT_HasCollision(Entity* first, Entity* second);
void ENT_ResolveCollision(Entity* collider, const Entity* collidee);
