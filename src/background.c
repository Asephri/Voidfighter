/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

#include "common.h"
#include "background.h"
#include "draw.h"

extern App app;

static int backgroundX;
static Star stars[MAX_STARS];
static SDL_Texture *background;

void initStars(void)
{
    int i;
    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % (SCREEN_HEIGHT - HUD_HEIGHT);
        stars[i].speed = 1 + rand() % 8;
    }
    //printf("Stars initialized.\n");
}

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

void doBackground(void)
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
    //printf("Background position updated: backgroundX = %d\n", backgroundX);
}

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
    //printf("Stars position updated.\n");
}

void drawStars(void)
{
    int i, c;

    for (i = 0; i < MAX_STARS; i++)
    {
        c = 32 * stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
    //printf("Stars rendered.\n");
}

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
