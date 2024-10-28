//
// Created by Anthony Lesch on 7/22/24.
//

#include <SDL_ttf.h>

#include "Text.h"
#include "Renderer.h"
#include "Resources.h"
#include "Utilities.h"

#define DEFAULT_FONT_SIZE 16
#define LARGE_FONT_SIZE 32

static void DrawTextInternal(const char* text, Vector2D position, TTF_Font* font);
static Texture* CreateTextureFromTextInternal(const char* text, TTF_Font* font, SDL_Color textColor);

static TTF_Font* s_SmallFont;
static TTF_Font* s_LargeFont;

static bool textInitialized;
static SDL_Color whiteColor = { 255, 255, 255 };
static SDL_Color blackColor = { 0, 0, 0 };

void TXT_InitializeText(void)
{
    if (textInitialized)
    {
        BB_LOG("Initialization of text system can only occur once.");
        return;
    }

    textInitialized = false;

    if (TTF_Init())
    {
        BB_LOG_ERROR("Unable to initialize TTF Library!: %s", TTF_GetError());
        return;
    }

    s_SmallFont = TTF_OpenFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
    if (s_SmallFont == NULL)
    {
        BB_LOG_ERROR("Unable to open TTF Font!: %s", TTF_GetError());

        TXT_DestroyTextSystem();
        return;
    }

    s_LargeFont = TTF_OpenFont(DEFAULT_FONT, LARGE_FONT_SIZE);
    if (s_LargeFont == NULL)
    {
        BB_LOG_ERROR("Unable to open TTF Font!: %s", TTF_GetError());

        TXT_DestroyTextSystem();
        return;
    }

    textInitialized = true;
}

void TXT_DestroyTextSystem(void)
{
    BB_LOG("Destroying text system.");

    textInitialized = false;

    TTF_CloseFont(s_SmallFont);
    TTF_Quit();
}

void TXT_DrawText(const char* text, const Vector2D position)
{
    DrawTextInternal(text, position, s_SmallFont);
}

Texture* TXT_CreateTextureFromText(const char* text)
{
    return CreateTextureFromTextInternal(text, s_SmallFont, whiteColor);
}

Texture* TXT_CreateTextureFromText_Black(const char* text)
{
    return CreateTextureFromTextInternal(text, s_SmallFont, blackColor);
}

Texture* TXT_CreateTextureFromText_Large(const char* text)
{
    return CreateTextureFromTextInternal(text, s_LargeFont, whiteColor);
}

static void DrawTextInternal(const char* text, const Vector2D position, TTF_Font* font)
{
    Texture* texture = CreateTextureFromTextInternal(text, font, whiteColor);
    if (texture == NULL) return;

    REN_DrawTexture(texture, position);
    REN_FreeTexture(texture);
}

static Texture* CreateTextureFromTextInternal(const char* text, TTF_Font* font, const SDL_Color textColor)
{
    if (!TTF_WasInit()) return NULL;

    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text, textColor);
    if (textSurface == NULL)
    {
        BB_LOG("Could not create text surface for text (%s): %s", text, TTF_GetError());
        return NULL;
    }

    Texture* texture = REN_LoadTextureFromSurface(textSurface);
    if (texture == NULL)
    {
        BB_LOG("Could not create texture from text surface: %s", TTF_GetError());
        SDL_FreeSurface(textSurface);

        return NULL;
    }

    SDL_FreeSurface(textSurface);

    int w, h;
    TTF_SizeText(font, text, &w, &h);

    texture->Width = w;
    texture->Height = h;

    return texture;
}
