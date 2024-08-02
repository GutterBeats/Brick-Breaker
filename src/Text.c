//
// Created by Anthony Lesch on 7/22/24.
//

#include <SDL_ttf.h>

#include "Text.h"

#include <stdbool.h>

#include "Renderer.h"
#include "Resources.h"

#define DEFAULT_FONT_SIZE 16
#define LARGE_FONT_SIZE 32

static void DrawTextInternal(const char* text, Vector2D position, TTF_Font* font);

static TTF_Font* s_SmallFont;
static TTF_Font* s_LargeFont;

static bool textInitialized;

void TXT_InitializeText(void)
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

    s_SmallFont = TTF_OpenFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
    if (s_SmallFont == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open TTF Font!: %s", TTF_GetError());

        TXT_DestroyTextSystem();
        return;
    }

    s_LargeFont = TTF_OpenFont(DEFAULT_FONT, LARGE_FONT_SIZE);
    if (s_LargeFont == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open TTF Font!: %s", TTF_GetError());

        TXT_DestroyTextSystem();
        return;
    }

    textInitialized = true;
}

void TXT_DestroyTextSystem(void)
{
    SDL_Log("Destroying text system.");

    textInitialized = false;

    TTF_CloseFont(s_SmallFont);
    TTF_Quit();
}

void TXT_DrawText(const char* text, const Vector2D position)
{
    DrawTextInternal(text, position, s_SmallFont);
}

void TXT_DrawText_Large(const char* text, const Vector2D position)
{
    DrawTextInternal(text, position, s_LargeFont);
}

static void DrawTextInternal(const char* text, const Vector2D position, TTF_Font* font)
{
    if (!TTF_WasInit()) return;

    // Set color to white
    const SDL_Color color = { 255, 255, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (textSurface == NULL)
    {
        SDL_Log("Could not create text surface for text (%s): %s", text, TTF_GetError());
        return;
    }

    Texture* texture = REN_LoadTextureFromSurface(textSurface);
    if (texture == NULL)
    {
        SDL_Log("Could not create texture from text surface: %s", TTF_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    int w, h;
    TTF_SizeText(font, text, &w, &h);

    texture->Width = w;
    texture->Height = h;

    REN_DrawTexture(texture, position);

    REN_FreeTexture(texture);
    SDL_FreeSurface(textSurface);
}
