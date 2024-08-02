//
// Created by Anthony Lesch on 7/16/24.
//

#pragma once

#include <stdbool.h>

#include "Collision.h"
#include "Renderer.h"

typedef struct entity
{
    int Health;
    int DamageGiven;
    const char* Name;

    float Speed;

    VectorF2D CurrentPosition;
    VectorF2D PreviousPosition;
    VectorF2D Size;

    Texture* Texture;
    CollisionVolume* CollisionVolume;

    bool IsEnabled;
} Entity;

Entity* ENT_CreateEntity(VectorF2D startPosition, const char* texturePath);

void ENT_MoveEntity(Entity* entity, VectorF2D direction, float deltaTime);
void ENT_DrawEntity(const Entity* entity);
void ENT_DestroyEntity(Entity* entity);
VectorF2D ENT_GetDirection(const Entity* entity);
