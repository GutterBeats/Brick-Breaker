//
// Created by Anthony Lesch on 8/19/24.
//

#pragma once

#include "Keyboard.h"

typedef void (*UserEventHandler)(void);

void EVT_InitializeEventSystem(void);
void EVT_DestroyEventSystem(void);
void EVT_PollEvents(void);

void EVT_UnbindAllUserEvents(void);
void EVT_UnbindUserEvent(USER_EVENT_TYPE eventType);
void EVT_BindUserEvent(USER_EVENT_TYPE eventType, UserEventHandler eventHandler);
