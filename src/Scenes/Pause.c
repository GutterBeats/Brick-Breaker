//
// Created by Anthony Lesch on 8/19/24.
//

#include "EventSystem.h"
#include "Types.h"

static void Initialize(void);
static void Draw(void);
static void Destroy(void);

Scene PauseScene = {
    .Initialize = Initialize,
    .Draw = Draw,
    .Destroy = Destroy
};

static void Initialize(void)
{

}

static void Draw(void)
{

}

static void Destroy(void)
{

}
