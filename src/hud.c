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

/* ----- Creating The Background ----- */

/* Intialising the hud. */
void initHud(void)
{
    hudX = 0;
    hudeffectsX = 0;
}

/* ---------- */

/* Rendering the hud. */
void drawHighscoreHud(void)
{
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = SCREEN_WIDTH;
	dest.h = SCREEN_HEIGHT;
	hud = loadTexture("gfx/highscorescreen.png");
	SDL_RenderCopy(app.renderer, hud, NULL, &dest);
	
}
/* ---------- */

/* Rendering the hud. */
void drawHud(void)
{
	SDL_Rect dest;

	dest.x = 0;
	dest.y = 0;
	dest.w = SCREEN_WIDTH;
	dest.h = SCREEN_HEIGHT;
	hud = loadTexture("gfx/hud.png");
	SDL_RenderCopy(app.renderer, hud, NULL, &dest);
	
}
/* ---------- */

/* Rendering the Hud. */
void drawHudEffects(void)
{
	SDL_Rect dest;
	dest.x = 0;
	dest.y = SCREEN_HEIGHT-HUD_HEIGHT;
    dest.w = 320;
	dest.h = 128;
	hudeffects = loadTexture("gfx/hudeffects.png");
	SDL_RenderCopy(app.renderer, hudeffects, NULL, &dest);
}
/* ---------- */