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

/* ----- Creating a Starfield Effect ----- */

/* Initialize the stars with random positions and speeds. */
void initStars(void)
{
    int i;
    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % (SCREEN_HEIGHT - HUD_HEIGHT);
        stars[i].speed = 1 + rand() % 8;
    }
    
    // Console message.
    //printf("Stars initialized.\n");
}

/* ----- Creating The Background ----- */

/* Initialize the background texture. */
void initBackground(void)
{
    background = loadTexture("gfx/background.png");
    backgroundX = 0;
    
    /*if (background == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background texture: %s\n", SDL_GetError());
    }
    else
    {
        // Console message.
        printf("Background initialized.\n");
    }*/
}

/* Update the background's horizontal position. */
void doBackground(void)
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
    
    // Console message.
    //printf("Background position updated: backgroundX = %d\n", backgroundX);
}

/* Update the stars' horizontal positions. */
void doStars(void)
{
    int i;
    
    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        if (stars[i].x < 0)
        {
            stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
    
    // Console message.
    //printf("Stars position updated.\n");
}

/* Render the stars on the screen. */
void drawStars(void)
{
    int i, c;

    for (i = 0; i < MAX_STARS; i++)
    {
        c = 32 * stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
    // Console message.
    //printf("Stars rendered.\n");
}

/* Render the background on the screen. */
void drawBackground(void)
{
    SDL_Rect dest;
    int x;

    for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = (SCREEN_HEIGHT - HUD_HEIGHT);

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }

    // Console message.
    //printf("Background rendered.\n");
}
