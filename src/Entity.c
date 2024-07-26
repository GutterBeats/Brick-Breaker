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

    int width, height;
    entity->Texture = LoadTexture(texturePath, &width, &height);

    if (entity->Texture == NULL)
    {
        return NULL;
    }

    entity->Bounds = (SDL_FRect){
        .x = x,
        .y = y,
        .w = (float)width,
        .h = (float)height
    };

    entity->Health = 0;
    entity->DamageGiven = 0;
    entity->IsEnabled = false;

    return entity;
}

void DrawEntity(const Entity* entity, const SDL_Rect* source)
{
    if (!entity->IsEnabled) return;

    DrawTextureF(entity->Texture, source, &entity->Bounds);
}

void DestroyEntity(Entity* entity)
{
    if (entity == NULL) return;

    if (entity->Texture != NULL)
    {
        SDL_DestroyTexture(entity->Texture);
    }

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
