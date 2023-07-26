/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "background.h"
#include "hud.h"
#include "highscores.h"
#include "stage.h"
#include "text.h"

/* Externs. */
extern App app;
extern Highscores highscores;

/* Functions. */
static void logic(void);
static void draw(void);
static int  highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);

/* ----- Creating the Highscore Table ----- */

/* Intialising the Highscores table. */

void initHighscoreTable(void)
{
    int i;

    memset(&highscores, 0, sizeof(Highscores));

    for (i = 0; i < NUM_HIGHSCORES ; i++)
    {
        highscores.highscore[i].score = NUM_HIGHSCORES - i;
    }
}

/* ---------- */

/* Intialising Highscores. */

void initHighscores(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;
    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
}

/* ---------- */

/* Local File logic. */

static void logic(void)
{
    doBackground();

    doStars();

    if (app.keyboard[SDL_SCANCODE_LCTRL])
    {
        initStage();
    }
}

/* ---------- */

/* Local Draw logic. */

static void draw(void)
{
    drawBackground();

    drawStars();

    drawHighscoreHud();

    drawHudEffects();

    drawHighscores();
}

/* ---------- */

/* Rendering Highscores. */

static void drawHighscores(void)
{
    int i, y;

    y= 290;

    drawText(450, 240, 255,255,255, "HIGHSCORES");

    for (i = 0 ; i <NUM_HIGHSCORES ; i++)
    {
        if (highscores.highscore[i].recent)
        {
            drawText(440, y, 255, 255, 0, "#%d ----- %03d", (i + 1), highscores.highscore[i].score);
        }
        else
        {
            drawText(430, y, 255, 255, 255, "#%d ----- %03d", (i + 1), highscores.highscore[i].score);
        }

        y += 40;

    }

    drawText(315, 600, 255,255,255, "PRESS RIGHT CTRL TO PLAY!");
}

/* ---------- */

/* Adding Highscores. */

void addHighscore(int score)
{
    Highscore newHighscores[NUM_HIGHSCORES + 1];
    int i;

    for (i = 0 ; i < NUM_HIGHSCORES ; i++)
    {
        newHighscores[i] = highscores.highscore[i];
        newHighscores[i].recent = 0;
    }

    newHighscores[NUM_HIGHSCORES].score = score;
    newHighscores[NUM_HIGHSCORES].recent = 1;

    qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);

    for (i = 0 ; i < NUM_HIGHSCORES ; i++)
    {
        highscores.highscore[i] = newHighscores[i];
    }
}

/* Returning a positive or negative interger for the qsort. */

static int highscoreComparator(const void *a, const void *b)
{
    Highscore *h1 = ((Highscore*)a);
    Highscore *h2 = ((Highscore*)b);

    return h2->score - h1->score;
}

/* ---------- */