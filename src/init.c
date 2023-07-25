/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Libraries. */
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

/* Headers. */
#include "common.h"
#include "init.h"

/* Externs. */
extern App app;

/* ----- Client Creation ----- */

/* Intialising Simple-DirectMedia-Layer. */
void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;
	windowFlags = 0;
	
    // Initialising SDL_Video libraries.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Initalising SDL_Mixer libraries.
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		printf("Couldn't initalize SDL Audio Mixer: %s\n", SDL_GetError());
		exit(1);
	}
	
	Mix_AllocateChannels(MAX_SND_CHANNELS);

    // Window creation.
	app.window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
	SDL_Surface* icon = IMG_Load("icon.ico"); SDL_SetWindowIcon(app.window, icon);
    // Makes the images smooth its gross.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    // Rendering the created window.
	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    // Intialising the IMG stuff.
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	// Disable cursor.
	SDL_ShowCursor(0);
}
/* ---------- */

/* Cleaning up the client. */
void cleanup(void)
{
	IMG_Quit();

	SDL_DestroyRenderer(app.renderer);

	SDL_DestroyWindow(app.window);

	SDL_Quit();
}
/* ---------- */