//
// Created by Anthony Lesch on 7/22/24.
//

#include <SDL_ttf.h>

#include "Text.h"

#include "Renderer.h"

#define FONT_PATH "resources/Fonts/Kenney Pixel Square.ttf"
#define FONT_SIZE 16

static TTF_Font* s_Font;

bool InitializeText(void)
{
    if (TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize TTF Library!: %s", TTF_GetError());
        return false;
    }

    s_Font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (s_Font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open TTF Font!: %s", TTF_GetError());
        return false;
    }

    return true;
}

void ShutdownText(void)
{
    TTF_Quit();
}

void DrawText(const char* text, const int x, const int y)
{
    if (!TTF_WasInit()) return;

    // Set color to black
    const SDL_Color color = { 0, 0, 0 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(s_Font, text, color);
    if (textSurface == NULL) return;

    SDL_Texture* texture = LoadTextureFromSurface(textSurface);
    if (texture == NULL)
    {
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
