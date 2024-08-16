//
// Created by Anthony Lesch on 8/14/24.
//

#include "ButtonContainer.h"
#include "Resources.h"
#include "Text.h"
#include "Utils.h"

#define BUTTON_PADDING 10

ButtonContainer* BC_CreateContainer(const VectorF2D position, const u8 containerSize)
{
    ButtonContainer* container = malloc(sizeof(ButtonContainer));
    if (container == NULL)
    {
        return NULL;
    }

    container->Buttons = calloc(containerSize, sizeof(Button*));
    if (container->Buttons == NULL)
    {
        free(container);
        return NULL;
    }

    container->ButtonTexture = REN_LoadTexture(BUTTON_TEXTURE);
    if (container->ButtonTexture == NULL)
    {
        free(container->Buttons);
        free(container);

        return NULL;
    }

    container->SelectedButtonTexture = REN_LoadTexture(SELECTED_BUTTON_TEXTURE);
    if (container->SelectedButtonTexture == NULL)
    {
        REN_FreeTexture(container->ButtonTexture);

        free(container->Buttons);
        free(container);

        return NULL;
    }

    container->Position = position;
    container->Size = UTL_MakeVectorF2D(
        container->SelectedButtonTexture->Width, (BUTTON_PADDING + container->SelectedButtonTexture->Height) * containerSize);

    container->SelectedIndex = 0;
    container->ButtonCount = 0;
    container->ContainerSize = containerSize;

    return container;
}

void BC_DestroyContainer(ButtonContainer* container)
{
    for (int i = 0; i < container->ContainerSize; ++i)
    {
        Button* button = container->Buttons[i];
        if (button == NULL) continue;

        REN_FreeTexture(button->Text);
        free(button);
    }

    REN_FreeTexture(container->ButtonTexture);
    REN_FreeTexture(container->SelectedButtonTexture);
    free(container->Buttons);
    free(container);
}

void BC_ChangeSelectedIndex(ButtonContainer* container, MovementDirection direction)
{
    switch (direction)
    {
        case UP:
        {
            if (container->SelectedIndex != 0)
            {
                container->SelectedIndex -= 1;
            }

            break;
        }
        case DOWN:
        {
            if (container->SelectedIndex < container->ButtonCount)
            {
                container->SelectedIndex += 1;
            }

            break;
        }
    }
}

void BC_ClickSelectedItem(const ButtonContainer* container)
{
    const Button* button = container->Buttons[container->SelectedIndex];
    button->ClickHandler();
}

void BC_DrawButtonContainer(const ButtonContainer* container)
{
    for (int i = 0; i < container->ButtonCount; ++i)
    {
        const Button* button = container->Buttons[i];
        const Texture* texture = i == container->SelectedIndex
            ? container->SelectedButtonTexture : container->ButtonTexture;

        const VectorF2D texturePosition = UTL_MakeVectorF2D(
            container->Position.X + container->Size.X / 2.f - texture->Width / 2.f,
            container->Position.Y + (BUTTON_PADDING + container->ButtonTexture->Height) * i);

        const float textX = texturePosition.X + texture->Width / 2.f - button->Text->Width / 2.f;
        const float textY = texturePosition.Y + texture->Height / 2.f - button->Text->Height / 2.f;

        REN_DrawTextureF(texture, texturePosition);
        REN_DrawTextureF(button->Text, UTL_MakeVectorF2D(textX, textY));
    }
}

void BC_CreateButton(ButtonContainer* container, const char* text, void (*clickHandler)())
{
    if (container->ButtonCount == container->ContainerSize) return;

    Button* button = malloc(sizeof(Button));
    if (button == NULL) return;

    button->Text = TXT_CreateTextureFromText_Black(text);
    if (button->Text == NULL)
    {
        free(button);
        return;
    }

    button->ClickHandler = clickHandler;
    container->Buttons[container->ButtonCount++] = button;
}
