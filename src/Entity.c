//
// Created by Anthony Lesch on 7/16/24.
//

#include "Entity.h"
#include "Renderer.h"
#include "Utils.h"

Entity* CreateEntity(const float x, const float y, const char* texturePath)
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

    entity->Bounds = (SDL_FRect){
        .x = x,
        .y = y,
        .w = entity->Texture->Width,
        .h = entity->Texture->Height
    };

    entity->Health = 0;
    entity->DamageGiven = 0;
    entity->IsEnabled = false;

    return entity;
}

void DrawEntity(const Entity* entity)
{
    if (!entity->IsEnabled) return;

    DrawTextureF(entity->Texture, entity->Bounds.x, entity->Bounds.y);
}

void DestroyEntity(Entity* entity)
{
    if (entity == NULL) return;

    FreeTexture(entity->Texture);
    free(entity);
}

bool HasTopCollision(const Entity* first, const Entity* second)
{
    return UTL_Between(second->Bounds.x, second->Bounds.x + second->Bounds.w, first->Bounds.x)
        // Checking with just > instead of >= here prevents stuttering of ball against paddle
        && first->Bounds.y + first->Bounds.w > second->Bounds.y;
}

bool IsColliding(const Entity* first, const Entity* second)
{
    return UTL_Between(second->Bounds.x, second->Bounds.x + second->Bounds.w, first->Bounds.x)
        && UTL_Between(second->Bounds.y, second->Bounds.y + second->Bounds.h, first->Bounds.y);
}
