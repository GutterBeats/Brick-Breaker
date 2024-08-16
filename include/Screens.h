/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Screens Functions Declarations (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#pragma once

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(float deltaTime);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
bool FinishTitleScreen(void);
void TitleEnterKeyPressed(void);

//----------------------------------------------------------------------------------
// Menu Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitMenuScreen(void);
void UpdateMenuScreen(float deltaTime);
void DrawMenuScreen(void);
void UnloadMenuScreen(void);
GameScreen FinishMenuScreen(void);
void MenuEnterKeyPressed(void);
void MenuUpKeyPressed(void);
void MenuDownKeyPressed(void);

//----------------------------------------------------------------------------------
// Options Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(float deltaTime);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
bool FinishOptionsScreen(void);
void OptionsEnterKeyPressed(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(float deltaTime);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
bool FinishGameplayScreen(void);
void GameplayEnterKeyPressed(void);

//----------------------------------------------------------------------------------
// Game Over Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameOverScreen(void);
void UpdateGameOverScreen(float deltaTime);
void DrawGameOverScreen(void);
void UnloadGameOverScreen(void);
bool FinishGameOverScreen(void);
void GameOverEnterKeyPressed(void);
