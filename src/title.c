/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include "common.h"
#include "background.h"
#include "draw.h"
#include "hud.h"
#include "highscores.h"
#include "stage.h"
#include "text.h"
#include "title.h"

extern App app;

static void logic(void);
static void draw(void);
static void drawTitle(void);

static SDL_Texture *voidfighter_titleTexture;
static int reveal = 0;
static int timeout;

void initTitle(void)
{
    printf("Initialising title.\n");

    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    voidfighter_titleTexture = loadTexture("gfx/voidfighter_title.png");

    timeout = FPS * 5;
}

static void logic(void)
{
    doBackground();

    doStars();

    doHud();

    if (reveal < SCREEN_HEIGHT)
    {
        reveal++;
    }

    if (--timeout <= -80)
    {
        initHighscores(); 
    }

    if (app.keyboard[SDL_SCANCODE_LCTRL])
    {
        initStage();
    }
}

static void draw(void)
{
    drawBackground();

    drawStars();

    drawHud();
    drawHudEffects();

    drawTitle();

    if (timeout % 40 < 20)
    {
        drawTextPOSITION(SCREEN_WIDTH / 2, 600, 255,255,255, TEXT_CENTER, "PRESS RCTRL TO PLAY!");
    }
}

static void drawTitle(void)
{
    //printf("Creating title.");
    SDL_Rect r;

    r.x = 0;
    r.y = 0;

    SDL_QueryTexture(voidfighter_titleTexture, NULL, NULL, &r.w, &r.h);

    r.h = MIN(reveal, r.h);

    blitRect(voidfighter_titleTexture, &r, (SCREEN_WIDTH / 2) - (r.w / 2), 100);
}