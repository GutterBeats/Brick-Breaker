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

void DrawTexture(SDL_Texture* texture, const SDL_Rect* source, const SDL_Rect* dest)
{
    if (SDL_RenderCopy(renderer, texture, source, dest))
    {
        SDL_Log("DrawTexture: There was an error copy the texture: %s", SDL_GetError());
    }
}

void DrawTextureF(SDL_Texture* texture, const SDL_Rect* source, const SDL_FRect* dest)
{
    if (SDL_RenderCopyF(renderer, texture, source, dest))
    {
        SDL_Log("DrawTextureF: There was an error copy the texture: %s", SDL_GetError());
    }
}

SDL_Texture* LoadTexture(const char* filePath)
{
    if ((IMG_Init(0) & flags) != flags)
    {
        SDL_Log("SDL Image library has not been initialized!");
        return NULL;
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (texture == NULL)
    {
        SDL_Log("Unable to load image (%s): %s", filePath, IMG_GetError());
    }

    return texture;
}

SDL_Texture* LoadTextureFromSurface(SDL_Surface* surface)
{
    return SDL_CreateTextureFromSurface(renderer, surface);
}
