/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include "common.h"
#include "background.h"
#include "highscores.h"
#include "stage.h"
#include "text.h"
#include "hud.h"
#include "title.h"

extern App app;
extern Highscores highscores;

static void logic(void);
static void draw(void);
static int highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);
static void doNameInput(void);
static void drawNameInput(void);
static void saveHighscores(const char* filename);

static Highscore* newHighscore;
static int cursorBlink;
static int timeout;

void loadHighscores(const char* filename, Highscores* highscores)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening highscores file: %s\n", filename);
        return;
    }

    int i = 0;
    while (i < NUM_HIGHSCORES && fscanf(file, "%49s %d", highscores->highscore[i].name, &highscores->highscore[i].score) == 2)
    {
        i++;
    }

    fclose(file);

    printf("Highscores loaded from file: %s\n", filename);
}

static void saveHighscores(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) 
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening highscores file for writing: %s\n", filename);
        return;
    }

    for (int i = 0; i < NUM_HIGHSCORES; i++) 
    {
        fprintf(file, "%s %d\n", highscores.highscore[i].name, highscores.highscore[i].score);
    }

    fclose(file);

    printf("Highscores saved to file: %s\n", filename);
}

void initHighscoreTable(void)
{
    int i;

    memset(&highscores, 0, sizeof(Highscores));
    for (i = 0; i < NUM_HIGHSCORES; i++)
    {
        highscores.highscore[i].score = NUM_HIGHSCORES - i;
        STRNCPY(highscores.highscore[i].name, "PLYR", MAX_SCORE_NAME_LENGTH);
    }

    newHighscore = NULL;

    cursorBlink = 0;
}

void initHighscores(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;
    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    loadHighscores("highscores.txt", &highscores);
}

static void logic(void)
{
    doBackground();

    doStars();

    drawHud();

    if (newHighscore != NULL)
    {
        doNameInput();
    }
    else
    {
        if(--timeout <= -500)
        {
            timeout = 0;
            initTitle();
        }
        if (app.keyboard[SDL_SCANCODE_LCTRL])
        {
            initStage();
        }
    }

    if (++cursorBlink >= FPS)
    {
        cursorBlink = 0;
    }
}

static void doNameInput(void)
{
    int i, n;
    char c;
    n = strlen(newHighscore->name);

    for (i = 0; i < strlen(app.inputText); i++)
    {
        c = toupper(app.inputText[i]);

        if (n < MAX_SCORE_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z')
        {
            newHighscore->name[n++] = c;
        }
    }

    if (n > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE])
    {
        newHighscore->name[--n] = '\0';

        app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
    }

    if (app.keyboard[SDL_SCANCODE_RETURN])
    {
        if (strlen(newHighscore->name) == 0)
        {
            STRNCPY(newHighscore->name, "PLYR", MAX_SCORE_NAME_LENGTH);
        }

        saveHighscores("highscores.txt");

        newHighscore = NULL;
    }
    
}

static void draw(void)
{
    drawBackground();

    drawStars();

    drawHud();

    drawHudEffects();

    if (newHighscore != NULL)
    {
        doHudInputscore();
        drawNameInput();
    }
    else
    {
        doHudscore();
        drawHighscores();
    }
}

static void drawNameInput(void)
{
    doHudInputscore();

    SDL_Rect r;

    drawText(408, 240, 255, 201, 14, "NEW HIGHSCORE!");

    drawText(392, 300, 255, 255, 255, "ENTER YOUR NAME:");

    drawTextPOSITION(SCREEN_WIDTH / 2, 360, 255, 242, 0, TEXT_CENTER, newHighscore->name);

    if (cursorBlink < FPS / 2)
    {
        r.x = (SCREEN_WIDTH / 2 + (strlen(newHighscore->name) * GLYPH_WIDTH / 2) + 5);
        r.y = 360;
        r.w = GLYPH_WIDTH;
        r.h = GLYPH_HEIGHT;

        SDL_SetRenderDrawColor(app.renderer, 0, 255, 201, 14);
        SDL_RenderFillRect(app.renderer, &r);
    }

    drawText(355, 448, 255, 255, 255, "HIT ENTER WHEN DONE.");
}

static void drawHighscores(void)
{
    doHudscore();
    int i, y, r, g, b;

    y = 290;

    drawText(450, 240, 255, 255, 255, "HIGHSCORES");

    for (i = 0 ; i < NUM_HIGHSCORES ; i++)
    {
        r = 255;
        b = 255;
        g = 255;

        if (highscores.highscore[i].recent)
        {
            b = 0;
        }

        drawText(300, y, r, g, b, "#%d. %-15s %03d", (i + 1), highscores.highscore[i].name, highscores.highscore[i].score);
        
        y += 40;

    }
    // Command to shoot to start game. This is a fixed issue. pelease remember to update this if it changes.
    drawText(315, 600, 255, 255, 255, "PRESS RIGHT CTRL TO PLAY!");
}

void addHighscore(int score)
{
    Highscore newHighscores[NUM_HIGHSCORES + 1];
    int i;

    memset(newHighscores, 0, sizeof(Highscore) * (NUM_HIGHSCORES + 1));

    for (i = 0 ; i < NUM_HIGHSCORES ; i++)
    {
        newHighscores[i] = highscores.highscore[i];
        newHighscores[i].recent = 0;
    }

    newHighscores[NUM_HIGHSCORES].score = score;
    newHighscores[NUM_HIGHSCORES].recent = 1;

    qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);

    newHighscore = NULL;

    for (i = 0 ; i < NUM_HIGHSCORES ; i++)
    {
        highscores.highscore[i] = newHighscores[i];

        if (highscores.highscore[i].recent)
        {
            newHighscore = &highscores.highscore[i];
        }
    }

    // Console message
    printf("Highscore added.\n");
}

static int highscoreComparator(const void *a, const void *b)
{
    Highscore *h1 = ((Highscore*)a);
    Highscore *h2 = ((Highscore*)b);
    return h2->score - h1->score;
}
