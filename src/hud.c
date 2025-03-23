/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include "common.h"
#include "hud.h"
#include "draw.h"

extern App app;
extern Stage stage;

static int hudX;
static int hudeffectsX;
static SDL_Texture *hud;
static SDL_Texture *hudeffects;
bool isLoaded = false;
bool isLoaded2 = false;
bool isLoaded3 = false;

void initHud(void)
{
    hudX = 0;
    hudeffectsX = 0;
    hudeffects = loadTexture("gfx/hudeffects.png");
    if (hudeffects == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load HUD effects texture: %s\n", SDL_GetError());
        return;
    }
    printf("HUD initialized.\n");
}

void doHud(void)
{
    if (!isLoaded)
    {
        hud = loadTexture("gfx/hud.png");
        if (hud == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load HUD texture: %s\n", SDL_GetError());
            return;
        }
        isLoaded = true;
        isLoaded2 = false;
        isLoaded3 = false;

        printf("Main HUD loaded.\n");
    }
}

void doHudscore(void)
{
    if (!isLoaded2)
    {
        hud = loadTexture("gfx/highscorescreen.png");
        if (hud == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load high score screen texture: %s\n", SDL_GetError());
            return;
        }
        isLoaded2 = true;
        isLoaded3 = false;
        isLoaded = false;

        printf("High score screen loaded.\n");
    }
}

void doHudInputscore(void)
{
    if (!isLoaded3)
    {
        hud = loadTexture("gfx/newhighscorescreen.png");
        if (hud == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load new high score screen texture: %s\n", SDL_GetError());
            return;
        }
        isLoaded3 = true;
        isLoaded2 = false;
        isLoaded = false;
        printf("New high score screen loaded.\n");
    }
}

void drawHud(void)
{
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.w = SCREEN_WIDTH;
    dest.h = SCREEN_HEIGHT;

    SDL_RenderCopy(app.renderer, hud, NULL, &dest);
}

void drawHudEffects(void)
{
    SDL_Rect dest;
    dest.x = 0;
    dest.y = SCREEN_HEIGHT - HUD_HEIGHT;
    dest.w = 320;
    dest.h = 128;

    SDL_RenderCopy(app.renderer, hudeffects, NULL, &dest);
}