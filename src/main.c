/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include "common.h"
#include "draw.h"
#include "init.h"
#include "title.h"
#include "input.h"
#include "main.h"

App app;
Highscores highscores;
Stage stage;

static void capFrameRate(long *then, float *remainder);

int main(int argc, char *argv[])
{
    long then;
    float remainder;

    freopen("consolelog.txt", "w", stdout);

	printf("----------------------------------------------------------------------------------------------------\n");

	printf("voidFighter - New Session\n");
	
	printf("Copyright (C) 2023-2025 Asephri.net. All rights reserved.\n");

	printf("consolelog.txt\n");
	
	printf("----------------------------------------------------------------------------------------------------\n");

    memset(&app, 0, sizeof(App));

    initSDL();

    atexit(cleanup);

    initGame();

    initTitle();

    then = SDL_GetTicks();
    remainder = 0;

    while (1)
    {
        prepareScene();

        doInput();

        app.delegate.logic();

        app.delegate.draw();

        presentScene();

        capFrameRate(&then, &remainder);
    }

    return 0;
}

static void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;

    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}