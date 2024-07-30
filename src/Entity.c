//
// Created by Anthony Lesch on 7/16/24.
//

#include "Entity.h"
#include "Renderer.h"
#include "Utils.h"

Entity* CreateEntity(const VectorF2D startPosition, const char* texturePath)
{
    Entity* entity = malloc(sizeof(Entity));
    if (entity == NULL)
    {
        SDL_Log("Unable to allocate memory for new entity.");
        return NULL;
    }

    entity->Texture = LoadTexture(texturePath);
    if (entity->Texture == NULL)
    {
        return NULL;
    }

    entity->Position = startPosition;
    entity->CurrentOverlap = GetZeroVectorF();
    entity->PreviousOverlap = GetZeroVectorF();
    entity->Size = (VectorF2D){ entity->Texture->Width, entity->Texture->Height };
    entity->HalfSize = (VectorF2D){ (float)entity->Texture->Width / 2.f, (float)entity->Texture->Height / 2.f };
    entity->Health = 0;
    entity->DamageGiven = 0;
    entity->IsEnabled = false;

    return entity;
}

void DrawEntity(const Entity* entity)
{
    if (!entity->IsEnabled) return;

    DrawTextureF(entity->Texture, entity->CurrentPosition);
}

void DestroyEntity(Entity* entity)
{
    if (entity == NULL) return;

    FreeTexture(entity->Texture);
    free(entity);
}

bool HasTopCollision(const Entity* first, const Entity* second)
{
    return UTL_Between(second->Position.X, second->Position.X + second->Size.X, first->Position.X)
        // Checking with just > instead of >= here prevents stuttering of ball against paddle
        && first->Position.Y + first->Size.X > second->Position.Y;
}

bool ENT_HasCollision(Entity* first, Entity* second)
{
    const float dx = fabs(first->Position.X - second->Position.X);
    const float dy = fabs(first->Position.Y - second->Position.Y);

    const float xOverlap = first->HalfSize.X + second->HalfSize.X - dx;
    const float yOverlap = first->HalfSize.Y + second->HalfSize.Y - dy;

    first->PreviousOverlap = first->CurrentOverlap;
    second->PreviousOverlap = second->CurrentOverlap;

    first->CurrentOverlap = (VectorF2D){ xOverlap, yOverlap };
    second->CurrentOverlap = (VectorF2D){ xOverlap, yOverlap };

    return xOverlap > 0 && yOverlap > 0;
}

void ENT_ResolveCollision(Entity* collider, const Entity* collidee)
{
    if (collidee->PreviousOverlap.X > 0)
    {
        // If ball Y position less than center of collision, came from top.
        if (collider->Position.Y <= collidee->Position.Y + collidee->HalfSize.Y)
        {
            collider->Position.Y -= collider->CurrentOverlap.Y;
        }

        // If ball position Y greater than center of collision, came from bottom.
        if (collider->Position.Y > collidee->Position.Y + collidee->HalfSize.Y)
        {
            collider->Position.Y += collider->CurrentOverlap.Y;
        }
    }

    if (collidee->PreviousOverlap.Y > 0)
    {
        // If ball X position less than center of collision, came from left.
        if (collider->Position.X <= collidee->Position.X + collidee->HalfSize.X)
        {
            collider->Position.X -= collider->CurrentOverlap.X;
        }

        // If ball position X greater than center of collision, came from right.
        if (collider->Position.X > collidee->Position.X + collidee->HalfSize.X)
        {
            collider->Position.X += collider->CurrentOverlap.X;
        }
    }
}
