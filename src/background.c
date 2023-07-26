/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "background.h"
#include "draw.h"

/* Externs. */
extern App app;

/* Global Variables. */
static int backgroundX;
static Star stars[MAX_STARS];
static SDL_Texture *background;

void initBackground(void)
{
    background = loadTexture("gfx/background.png");
    backgroundX = 0;
}

/* ----- Creating a Starfield Effect ----- */

/* Intialising Stars. */
void initStars(void)
{
    int i;
    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % (SCREEN_HEIGHT - HUD_HEIGHT)  ;
        stars[i].speed = 1 + rand() % 8;
    }   
}
/* ---------- */

/* ----- Creating The Background ----- */

/* Background Behavour. */
void doBackground(void)
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}
/* ---------- */

/* The Stars behaviour. */
void doStars(void)
{
    int i;
    
    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        if (stars[i].x <0)
        {
            stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
}
/* ---------- */

/* Rendering the Stars. */
void drawStars(void)
{
	int i, c;

	for (i = 0; i < MAX_STARS; i++)
	{
		c = 32 * stars[i].speed;

		SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}
/* ---------- */

/* Rendering the background. */
void drawBackground(void)
{
	SDL_Rect dest;
	int      x;

	for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
	{
		dest.x = x;
		dest.y = 0;
		dest.w = SCREEN_WIDTH;
		dest.h = (SCREEN_HEIGHT - HUD_HEIGHT)  ;

		SDL_RenderCopy(app.renderer, background, NULL, &dest);
	}
}
/* ---------- */