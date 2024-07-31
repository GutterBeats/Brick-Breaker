//
// Created by Anthony Lesch on 7/16/24.
//

#include "Entity.h"

#include "Game.h"
#include "Renderer.h"
#include "Utils.h"

Entity* ENT_CreateInvisibleEntity(const VectorF2D startPosition, const VectorF2D size)
{
    Entity* entity = malloc(sizeof(Entity));
    if (entity == NULL)
    {
        SDL_Log("Unable to allocate memory for new entity.");
        return NULL;
    }

    entity->CurrentPosition = startPosition;
    entity->CurrentOverlap = UTL_GetZeroVectorF();
    entity->PreviousOverlap = UTL_GetZeroVectorF();
    entity->Size = size;
    entity->HalfSize = UTL_ScaleVectorF2D(entity->Size, 0.5f);
    entity->Health = 0;
    entity->DamageGiven = 0;
    entity->IsEnabled = false;

    return entity;
}

Entity* ENT_CreateEntity(const VectorF2D startPosition, const char* texturePath)
{
    Texture* texture = REN_LoadTexture(texturePath);
    if (texture == NULL)
    {
        return NULL;
    }

    Entity* entity = ENT_CreateInvisibleEntity(startPosition,
        UTL_MakeVectorF2D(texture->Width, texture->Height));

    if (entity == NULL)
    {
        REN_FreeTexture(texture);
        return NULL;
    }

    entity->Texture = texture;

    return entity;
}

void ENT_MoveEntity(Entity* entity, const VectorF2D direction, const float deltaTime)
{
    entity->PreviousPosition = entity->CurrentPosition;

    const VectorF2D newDirection = UTL_AddVectorF2D(entity->CurrentPosition, direction);

    const VectorF2D velocity = UTL_ScaleVectorF2D(
        UTL_SubtractVectorF2D(newDirection, entity->PreviousPosition), entity->Speed * deltaTime);

    entity->CurrentPosition = UTL_AddVectorF2D(entity->CurrentPosition, velocity);
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
    free(entity);
}

bool ENT_HasCollision(Entity* first, Entity* second)
{
    const float dx = fabs(first->CurrentPosition.X - second->CurrentPosition.X);
    const float dy = fabs(first->CurrentPosition.Y - second->CurrentPosition.Y);

    const float xOverlap = first->HalfSize.X + second->HalfSize.X - dx;
    const float yOverlap = first->HalfSize.Y + second->HalfSize.Y - dy;

    first->PreviousOverlap = first->CurrentOverlap;
    second->PreviousOverlap = second->CurrentOverlap;

    first->CurrentOverlap = UTL_MakeVectorF2D(xOverlap, yOverlap);
    second->CurrentOverlap = UTL_MakeVectorF2D(xOverlap, yOverlap);

    return xOverlap >= 0 && yOverlap >= 0;
}

void ENT_ResolveCollision(Entity* collider, const Entity* collidee)
{
    if (collidee->PreviousOverlap.X >= 0)
    {
        // If ball Y position less than center of collision, came from top.
        if (collider->CurrentPosition.Y <= collidee->CurrentPosition.Y + collidee->HalfSize.Y)
        {
            collider->CurrentPosition.Y -= collider->CurrentOverlap.Y;
        }

        // If ball position Y greater than center of collision, came from bottom.
        if (collider->CurrentPosition.Y >= collidee->CurrentPosition.Y + collidee->HalfSize.Y)
        {
            collider->CurrentPosition.Y += collider->CurrentOverlap.Y;
        }
    }

    if (collidee->PreviousOverlap.Y >= 0)
    {
        // If ball X position less than center of collision, came from left.
        if (collider->CurrentPosition.X <= collidee->CurrentPosition.X + collidee->HalfSize.X)
        {
            collider->CurrentPosition.X -= collider->CurrentOverlap.X;
        }

        // If ball position X greater than center of collision, came from right.
        if (collider->CurrentPosition.X >= collidee->CurrentPosition.X + collidee->HalfSize.X)
        {
            collider->CurrentPosition.X += collider->CurrentOverlap.X;
        }
    }
}
