/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "hud.h"
#include "draw.h"

/* Externs. */
extern App app;
extern Stage stage;

/* Global Variables. */
static int hudX;
static int hudeffectsX;
static SDL_Texture *hud;
static SDL_Texture *hudeffects;
bool isLoaded = false;
bool isLoaded2 = false;
bool isLoaded3 = false;

/* ----- Creating The Background ----- */

/* Initialize the HUD textures and set initial positions. */
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
    // Console message.
    printf("HUD initialized.\n");
}

/* ---------- */

/* Load the main HUD texture. */
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
        // Console message.
        printf("Main HUD loaded.\n");
    }
}

/* Load the high score screen texture. */
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
        // Console message.
        printf("High score screen loaded.\n");
    }
}

/* Load the new high score screen texture. */
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
        // Console message.
        printf("New high score screen loaded.\n");
    }
}

/* Rendering the HUD. */
void drawHud(void)
{
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.w = SCREEN_WIDTH;
    dest.h = SCREEN_HEIGHT;

    SDL_RenderCopy(app.renderer, hud, NULL, &dest);
}

/* ---------- */

/* Rendering the HUD effects. */
void drawHudEffects(void)
{
    SDL_Rect dest;
    dest.x = 0;
    dest.y = SCREEN_HEIGHT - HUD_HEIGHT;
    dest.w = 320;
    dest.h = 128;

    SDL_RenderCopy(app.renderer, hudeffects, NULL, &dest);
}