//
// Created by Anthony Lesch on 8/14/24.
//

#pragma once

#include "Renderer.h"

typedef enum movement_direction
{
    UP, DOWN
} MovementDirection;

typedef struct button
{
    Texture* Text;

    void (*ClickHandler)();
} Button;

typedef struct button_container
{
    VectorF2D Position;
    VectorF2D Size;

    u8 SelectedIndex;
    u8 ButtonCount;
    u8 ContainerSize;

    Texture* ButtonTexture;
    Texture* SelectedButtonTexture;

    Button** Buttons;
} ButtonContainer;

ButtonContainer* BC_CreateContainer(VectorF2D position, u8 containerSize);
void BC_DestroyContainer(ButtonContainer* container);
void BC_ChangeSelectedIndex(ButtonContainer* container, MovementDirection direction);
void BC_ClickSelectedItem(const ButtonContainer* container);
void BC_DrawButtonContainer(const ButtonContainer* container);
void BC_CreateButton(ButtonContainer* container, const char* text, void (*clickHandler)());
