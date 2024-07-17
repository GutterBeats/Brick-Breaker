//
// Created by Anthony Lesch on 7/16/24.
//

#include "Entity.h"
#include "Renderer.h"

Entity* CreateEntity(VectorF2D position, VectorF2D size, const char* texturePath)
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

    entity->Position = position;
    entity->Size = size;

    return entity;
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

SDL_FRect GetEntityRectangle(Entity* entity)
{
    return (SDL_FRect){
      .x = entity->Position.X,
      .y = entity->Position.Y,
      .w = entity->Size.X,
      .h = entity->Size.Y
    };
}
