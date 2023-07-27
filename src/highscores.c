/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "background.h"
#include "highscores.h"
#include "stage.h"
#include "text.h"
#include "hud.h"

/* Externs. */
extern App app;
extern Highscores highscores;

/* Functions. */
static void logic(void);
static void draw(void);
static int highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);
static void doNameInput(void);
static void drawNameInput(void);
static void saveHighscores(const char* filename);

/* Variables. */
static Highscore* newHighscore;
static int cursorBlink;

/* Creating Highscores metadata. */

/* Loading Highscores from file. */
void loadHighscores(const char* filename, Highscores* highscores)
{
    FILE* file = fopen(filename, "r"); // Open the file in read mode.
    if (file == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening highscores file: %s\n", filename);
        return;
    }

    int i = 0; // Counter to keep track of the highscores read.
    while (i < NUM_HIGHSCORES && fscanf(file, "%49s %d", highscores->highscore[i].name, &highscores->highscore[i].score) == 2)
    {
        // Read each line from the file and store the name and score in the 'highscores' array
        i++; // Move to the next position in the 'highscores' array
    }

    fclose(file);

    // Console message
    printf("Highscores loaded from file: %s\n", filename);
}

/* Saving Highscores to file. */
static void saveHighscores(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) 
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening highscores file for writing: %s\n", filename);
        return;
    }

    // Write highscores to the file
    for (int i = 0; i < NUM_HIGHSCORES; i++) 
    {
        fprintf(file, "%s %d\n", highscores.highscore[i].name, highscores.highscore[i].score);
    }

    fclose(file);

    // Console message
    printf("Highscores saved to file: %s\n", filename);
}

/* ----- Creating the Highscore Table ----- */

/* Initializing the Highscores table. */
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

/* Initializing Highscores. */
void initHighscores(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;
    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    loadHighscores("highscores.txt", &highscores);
}

/* Local file logic. */
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

/* User name input. */
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

        saveHighscores("highscores.txt"); // Save data.

        newHighscore = NULL;
    }
    
}

/* Local draw logic. */
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

/* Rendering Name Input. */
static void drawNameInput(void)
{
    doHudInputscore();

    SDL_Rect r;

    drawText(408, 240, 255, 55, 255, "NEW HIGHSCORE!");

    drawText(392, 300, 255, 255, 255, "ENTER YOUR NAME:");

    drawTextPOSITION(SCREEN_WIDTH / 2, 360, 128, 255, 128, TEXT_CENTER, newHighscore->name);

    if (cursorBlink < FPS / 2)
    {
        r.x = (SCREEN_WIDTH / 2 + (strlen(newHighscore->name) * GLYPH_WIDTH / 2) + 5);
        r.y = 360;
        r.w = GLYPH_WIDTH;
        r.h = GLYPH_HEIGHT;

        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(app.renderer, &r);
    }

    drawText(355, 448, 255, 255, 255, "HIT ENTER WHEN DONE.");
}

/* Rendering Highscores. */
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

    drawText(315, 600, 255, 255, 255, "PRESS RIGHT CTRL TO PLAY!");
}

/* Adding Highscores. */
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

/* Returning a positive or negative integer for the qsort. */
static int highscoreComparator(const void *a, const void *b)
{
    Highscore *h1 = ((Highscore*)a);
    Highscore *h2 = ((Highscore*)b);
    return h2->score - h1->score;
}
