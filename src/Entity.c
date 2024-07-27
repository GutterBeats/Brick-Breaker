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
        SDL_Log("Unable to allocated memory for new entity.");
        return NULL;
    }

    entity->Texture = LoadTexture(texturePath);
    if (entity->Texture == NULL)
    {
        return NULL;
    }

    entity->CurrentPosition = startPosition;
    entity->PreviousPosition = GetZeroVectorF();
    entity->Size = (VectorF2D){ entity->Texture->Width, entity->Texture->Height };
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
    return UTL_Between(second->CurrentPosition.X, second->CurrentPosition.X + second->Size.X, first->CurrentPosition.X)
        // Checking with just > instead of >= here prevents stuttering of ball against paddle
        && first->CurrentPosition.Y + first->Size.X > second->CurrentPosition.Y;
}
