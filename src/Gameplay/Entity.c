//
// Created by Anthony Lesch on 7/16/24.
//

#include "Entity.h"
#include "Collision.h"
#include "Game.h"
#include "Renderer.h"
#include "Utils.h"

Entity* ENT_CreateEntity(const VectorF2D startPosition, const char* texturePath)
{
    Entity* entity = malloc(sizeof(Entity));
    if (entity == NULL)
    {
        BB_LOG("Unable to allocate memory for new entity.");
        return NULL;
    }

    Texture* texture = REN_LoadTexture(texturePath);
    if (texture == NULL)
    {
        free(entity);
        return NULL;
    }

    entity->CurrentPosition = startPosition;
    entity->Size = UTL_MakeVectorF2D(texture->Width, texture->Height);
    entity->Health = 0;
    entity->DamageGiven = 0;
    entity->IsEnabled = false;
    entity->Texture = texture;

    return entity;
}

void ENT_MoveEntity(Entity* entity, const VectorF2D direction, const float deltaTime)
{
    entity->PreviousPosition = entity->CurrentPosition;

    const VectorF2D newDirection = UTL_AddVectorF2D(entity->CurrentPosition, UTL_NormalizeVectorF2D(direction));

    const VectorF2D velocity = UTL_ScaleVectorF2D(
        UTL_SubtractVectorF2D(newDirection, entity->PreviousPosition), entity->Speed * deltaTime);

    entity->CurrentPosition = UTL_AddVectorF2D(entity->CurrentPosition, velocity);
    entity->CollisionVolume->Position = entity->CurrentPosition;
}

void ENT_DrawEntity(const Entity* entity)
{
    if (!entity->IsEnabled) return;

    if (GAM_GetShowDebug())
    {
        REN_DrawRectangleF(entity->CurrentPosition, entity->Size);
    }

    REN_DrawTextureF(entity->Texture, entity->CurrentPosition);
}

void ENT_DestroyEntity(Entity* entity)
{
    if (entity == NULL) return;

    REN_FreeTexture(entity->Texture);

    if (entity->CollisionVolume != NULL)
    {
        free(entity->CollisionVolume);
    }

    free(entity);
}

VectorF2D ENT_GetDirection(const Entity* entity)
{
    const VectorF2D direction = UTL_SubtractVectorF2D(entity->CurrentPosition, entity->PreviousPosition);
    return UTL_NormalizeVectorF2D(direction);
}
