//
// Created by Anthony Lesch on 7/22/24.
//

#include <SDL_ttf.h>

#include "Text.h"
#include "Renderer.h"
#include "Resources.h"

static TTF_Font* s_Font;
static bool textInitialized;

void InitializeText(void)
{
    if (textInitialized)
    {
        SDL_Log("Initialization of text system can only occur once.");
        return;
    }

    textInitialized = false;

    if (TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize TTF Library!: %s", TTF_GetError());
        return;
    }

    s_Font = TTF_OpenFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
    if (s_Font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open TTF Font!: %s", TTF_GetError());

        DestroyTextSystem();
        return;
    }

    textInitialized = true;
}

void DestroyTextSystem(void)
{
    SDL_Log("Destroying text system.");

    textInitialized = false;

    TTF_CloseFont(s_Font);
    TTF_Quit();
}

void DrawText(const char* text, const int x, const int y)
{
    if (!TTF_WasInit()) return;

    // Set color to black
    const SDL_Color color = { 0, 0, 0 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(s_Font, text, color);
    if (textSurface == NULL)
    {
        SDL_Log("Could not create text surface for text (%s): %s", text, TTF_GetError());
        return;
    }

    SDL_Texture* texture = LoadTextureFromSurface(textSurface);
    if (texture == NULL)
    {
        SDL_Log("Could not create texture from text surface: %s", TTF_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    int w, h;
    TTF_SizeText(s_Font, text, &w, &h);

    const SDL_Rect rect = { x, y, w, h };

    DrawTexture(texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(textSurface);
}
