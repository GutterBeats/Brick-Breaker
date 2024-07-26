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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);

    return 1;
}

void DestroyRenderer(void)
{
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
}

void BeginDrawing(void)
{
    SDL_RenderClear(renderer);
}

void FinishDrawing(void)
{
    SDL_RenderPresent(renderer);
}

void DrawTexture(const Texture* texture, const int x, const int y)
{
    const SDL_Rect dest = { x, y, texture->Width, texture->Height };

    if (SDL_RenderCopy(renderer, texture->Data, NULL, &dest))
    {
        SDL_Log("DrawTexture: There was an error copy the texture: %s", SDL_GetError());
    }
}

void DrawTexture_Alpha(const Texture* texture, const int x, const int y, const float alpha)
{
    const SDL_Rect dest = { x, y, texture->Width, texture->Height };

    SDL_SetTextureBlendMode(texture->Data, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture->Data, alpha * 255);

    if (SDL_RenderCopy(renderer, texture->Data, NULL, &dest))
    {
        SDL_Log("DrawTexture: There was an error copy the texture: %s", SDL_GetError());
    }
}

void DrawTextureF(const Texture* texture, const float x, const float y)
{
    const SDL_FRect dest = { x, y, texture->Width, texture->Height };

    if (SDL_RenderCopyF(renderer, texture->Data, NULL, &dest))
    {
        SDL_Log("DrawTextureF: There was an error copy the texture: %s", SDL_GetError());
    }
}

void DrawTextureF_Alpha(const Texture* texture, const float x, const float y, const float alpha)
{
    const SDL_FRect dest = { x, y, texture->Width, texture->Height };

    SDL_SetTextureBlendMode(texture->Data, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture->Data, alpha * 255);

    if (SDL_RenderCopyF(renderer, texture->Data, NULL, &dest))
    {
        SDL_Log("DrawTexture: There was an error copy the texture: %s", SDL_GetError());
    }
}

Texture* LoadTexture(const char* filePath)
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
        FreeTexture(texture);

        return NULL;
    }

    return texture;
}

Texture* LoadTextureFromSurface(SDL_Surface* surface)
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

void FreeTexture(Texture* texture)
{
    if (texture == NULL) return;

    SDL_DestroyTexture(texture->Data);
    free(texture);
}
