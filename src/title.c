/* 
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "background.h"
#include "draw.h"
#include "hud.h"
#include "highscores.h"
#include "stage.h"
#include "text.h"
#include "title.h"

/* Externs. */
extern App app;

/* Functions. */
static void logic(void);
static void draw(void);
static void drawTitle(void);

/* Variables. */
static SDL_Texture *voidfighter_titleTexture;
static int reveal = 0;
static int timeout;

/* Intialising the titlescreen. */
void initTitle(void)
{
    // Console message.
    printf("Initialising title.\n");
    // assiging delegate functions locally.
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    // Allocating memory blocks.
    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS); // clearing keyboard input.

    voidfighter_titleTexture = loadTexture("gfx/voidfighter_title.png");

    // Title screentime out.

    timeout = FPS * 5; // Five seconds.
}

/* Local file logic. */
static void logic(void)
{
    printf("Local logic loaded.\n");
    doBackground();

    doStars();

    doHud();

    if (reveal < SCREEN_HEIGHT) // Prevent overflow wraparound.
    {
        reveal++; // Incrementing reveal for title.
    }

    if (--timeout <= -80) // decrementing timeout.
    {
        initHighscores(); 
    }

    if (app.keyboard[SDL_SCANCODE_LCTRL])
    {
        initStage();
    }
}

/* Local draw logic. */
static void draw(void)
{
    printf("draw logic for title.");
    drawBackground();

    drawStars();

    drawHud();
    drawHudEffects();

    drawTitle();

    if (timeout % 40 < 20) // Calculating modulus to the degree of 40. if less than 20 ect.
    {
        drawTextPOSITION(SCREEN_WIDTH / 2, 600, 255,255,255, TEXT_CENTER, "PRESS RCTRL TO PLAY!");
    }
}

/* Title rendering. */
static void drawTitle(void)
{
    printf("Creating title.");
    SDL_Rect r;

    r.x = 0;
    r.y = 0;

    SDL_QueryTexture(voidfighter_titleTexture, NULL, NULL, &r.w, &r.h);

    r.h = MIN(reveal, r.h); // According to the value of reveal.

    blitRect(voidfighter_titleTexture, &r, (SCREEN_WIDTH / 2) - (r.w / 2), 100);
}