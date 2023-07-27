/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/
/* Libraries. */
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

/* Headers. */
#include "common.h"
#include "draw.h"
#include "background.h"
#include "highscores.h"
#include "init.h"
#include "sound.h"
#include "text.h"
#include "hud.h"

/* Externs. */
extern App app;

/* ----- Client Creation ----- */

/* Initializing Simple-DirectMedia-Layer (SDL). */
void initSDL(void)
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    // Initialize SDL_Video libraries.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize SDL_Mixer libraries.
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("Couldn't initalize SDL Audio Mixer: %s\n", SDL_GetError());
        exit(1);
    }

    Mix_AllocateChannels(MAX_SND_CHANNELS);

    // Create the window.
    app.window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    SDL_Surface* icon = IMG_Load("icon.ico"); SDL_SetWindowIcon(app.window, icon);

    // Set rendering hint for smooth images.
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    // Create the renderer.
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    // Initialize the IMG subsystem for loading PNG and JPG images.
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    // Disable the cursor.
    SDL_ShowCursor(0);

    // Console message.
    printf("SDL Initialized.\n");
}

/* Initialize the game components. */
void initGame(void)
{
    initBackground();

    initStars();

    initSounds();

    initFonts();

    initHud();
	
    initHighscoreTable();

    loadMusic("music/voidfighter - Track 01 (deepspace-01).ogg");

    playMusic(1);

    // Console message.
    printf("Game initialized.\n");
}

/* Cleaning up the client. */
void cleanup(void)
{
    IMG_Quit();

    SDL_DestroyRenderer(app.renderer);

    SDL_DestroyWindow(app.window);

	fclose(stdout);

    SDL_Quit();

    // Console message.
    printf("Cleaned up and quit SDL.\n");
}
