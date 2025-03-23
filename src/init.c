/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "common.h"
#include "draw.h"
#include "background.h"
#include "highscores.h"
#include "init.h"
#include "sound.h"
#include "text.h"
#include "hud.h"

extern App app;


void initSDL(void)
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("Couldn't initalize SDL Audio Mixer: %s\n", SDL_GetError());
        exit(1);
    }

    Mix_AllocateChannels(MAX_SND_CHANNELS);

    app.window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    SDL_Surface* icon = IMG_Load("icon.ico"); SDL_SetWindowIcon(app.window, icon);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_ShowCursor(0);

    printf("SDL Initialized.\n");
}

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

    printf("Game initialized.\n");
}

void cleanup(void)
{
    IMG_Quit();

    SDL_DestroyRenderer(app.renderer);

    SDL_DestroyWindow(app.window);

	fclose(stdout);

    SDL_Quit();

    printf("Cleaned up and quit SDL.\n");
}
