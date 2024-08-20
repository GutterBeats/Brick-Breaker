//
// Created by Anthony Lesch on 8/19/24.
//

#include "EventSystem.h"
#include "Game.h"
#include "Keyboard.h"
#include "Utils.h"

static UserEventHandler* eventHandlers;

void EVT_InitializeEventSystem(void)
{
    eventHandlers = malloc(USER_EVENT_COUNT * sizeof(UserEventHandler));
    ASSERT_NOTNULL(eventHandlers, "Event Handlers");
}

void EVT_DestroyEventSystem(void)
{
    free(eventHandlers);
}

void EVT_PollEvents(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            GAM_SetIsGameRunning(false);
            break;
        }

        if (event.type == SDL_USEREVENT
            && IS_VALID_USER_EVENT(event.user.code))
        {
            const USER_EVENT_TYPE userEventType = event.user.code;
            if (eventHandlers[userEventType] != NULL)
            {
                eventHandlers[userEventType]();
            }

            continue;
        }

        KBD_HandleEvent(&event);
    }
}

void EVT_BindUserEvent(const USER_EVENT_TYPE eventType, const UserEventHandler eventHandler)
{
    if (!IS_VALID_USER_EVENT(eventType)) return;

    eventHandlers[eventType] = eventHandler;
}
