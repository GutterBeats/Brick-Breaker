//
// Created by Anthony Lesch on 7/16/24.
//

#include "Renderer.h"

#include <SDL.h>
#include <SDL_image.h>

static SDL_Renderer* renderer;
static int flags = IMG_INIT_JPG | IMG_INIT_PNG;

u8 InitializeRenderer(SDL_Window* window)
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        SDL_Log("Unable to create a renderer!: %s", SDL_GetError());
        return 0;
    }

    if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
    {
        SDL_Log("Unable to initialize image library for JPG!: %s", IMG_GetError());
        return 0;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_Log("Unable to initialize image library for PNG!: %s", IMG_GetError());
        return 0;
    }

    return 1;
}

void REN_DestroyRenderer(void)
{
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
}

void REN_BeginDrawing(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
    SDL_RenderClear(renderer);
}

void REN_FinishDrawing(void)
{
    SDL_RenderPresent(renderer);
}

void REN_DrawTexture(const Texture* texture, const Vector2D position)
{
    if (texture == NULL) return;

    const SDL_Rect dest = { position.X, position.Y, texture->Width, texture->Height };

    if (SDL_RenderCopy(renderer, texture->Data, NULL, &dest))
    {
        SDL_Log("DrawTexture: There was an error copy the texture: %s", SDL_GetError());
    }
}

void REN_DrawTexture_Alpha(const Texture* texture, const Vector2D position, const float alpha)
{
    if (texture == NULL) return;

    const SDL_Rect dest = { position.X, position.Y, texture->Width, texture->Height };

    SDL_SetTextureBlendMode(texture->Data, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture->Data, alpha * 255);

    if (SDL_RenderCopy(renderer, texture->Data, NULL, &dest))
    {
        SDL_Log("DrawTexture: There was an error copy the texture: %s", SDL_GetError());
    }
}

void REN_DrawRectangleF(const VectorF2D position, const VectorF2D size)
{
    const SDL_FRect rect = (SDL_FRect){
        .x = position.X, .y = position.Y,
        .w = size.X, .h = size.Y
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRectF(renderer, &rect);
}

void REN_DrawTextureF(const Texture* texture, const VectorF2D position)
{
    if (texture == NULL) return;

    const SDL_FRect dest = { position.X, position.Y, texture->Width, texture->Height };

    if (SDL_RenderCopyF(renderer, texture->Data, NULL, &dest))
    {
        SDL_Log("DrawTextureF: There was an error copy the texture: %s", SDL_GetError());
    }
}

Texture* REN_LoadTexture(const char* filePath)
{
    if ((IMG_Init(0) & flags) != flags)
    {
        SDL_Log("SDL Image library has not been initialized!");
        return NULL;
    }

    Texture* texture = malloc(sizeof(Texture));
    if (texture == NULL)
    {
        SDL_Log("Unable to allocate memory for texture (%s).", filePath);
        return NULL;
    }

    texture->Data = IMG_LoadTexture(renderer, filePath);
    if (texture->Data == NULL)
    {
        SDL_Log("Unable to load image (%s): %s", filePath, IMG_GetError());
        free(texture);

        return NULL;
    }

    if (SDL_QueryTexture(texture->Data, NULL, NULL, &texture->Width, &texture->Height))
    {
        SDL_Log("Unable to query texture for width and height!: %s", SDL_GetError());
        REN_FreeTexture(texture);

        return NULL;
    }

    return texture;
}

Texture* REN_LoadTextureFromSurface(SDL_Surface* surface)
{
    Texture* texture = malloc(sizeof(Texture));
    if (texture == NULL)
    {
        SDL_Log("Unable to allocate memory for texture.");
        return NULL;
    }

    texture->Data = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture->Data == NULL)
    {
        SDL_Log("Unable to load image: %s", IMG_GetError());
        free(texture);

        return NULL;
    }

    return texture;
}

void REN_FreeTexture(Texture* texture)
{
    if (texture == NULL) return;

    SDL_DestroyTexture(texture->Data);
    free(texture);
}
