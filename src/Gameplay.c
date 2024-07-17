//
// Created by Anthony Lesch on 7/16/24.
//

#include <math.h>

#include "Keyboard.h"
#include "Types.h"
#include "Renderer.h"
#include "Entity.h"
#include "MathUtils.h"

#define PLAYER_TEXTURE "resources/Textures/paddleBlue.png"

//----------------------------------------------------------------------------------
// Gameplay Screen Variables
//----------------------------------------------------------------------------------
static int windowWidth = 0;
static int windowHeight = 0;
static float playerWidth = 104;
static float playerHeight = 24;
static float interpolationSpeed = 0.9f;
static float movementSpeed = 350.f;
static float destinationX;

static Entity* player;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions
//----------------------------------------------------------------------------------
void InitGameplayScreen(void)
{
    GetScreenDimensions(&windowWidth, &windowHeight);

    destinationX = ((float)windowWidth / 2.f) - (playerWidth / 2.f);

    player = CreateEntity(
            (VectorF2D) {destinationX, (float) windowHeight - (playerHeight * 2.f)},
            (VectorF2D) {playerWidth, playerHeight},
            PLAYER_TEXTURE);
}

void UpdateGameplayScreen(float deltaTime)
{
    if (player == NULL) return;

    if (KBD_IsLeftKeyDown())
    {
        destinationX -= movementSpeed * deltaTime;
    }
    else if (KBD_IsRightKeyDown())
    {
        destinationX += movementSpeed * deltaTime;
    }

    destinationX = clamp(0.f, (float)windowWidth, destinationX);

    float interpolation = powf(interpolationSpeed, deltaTime * 60);

    player->Position.X = lerp(destinationX, player->Position.X, interpolation);
}

void DrawGameplayScreen(void)
{
    if (player == NULL) return;

    SDL_FRect rect = GetEntityRectangle(player);
    DrawTexture(player->Texture, NULL, &rect);
}

void UnloadGameplayScreen(void)
{
    DestroyEntity(player);
}

bool FinishGameplayScreen(void)
{
    return false;
}
