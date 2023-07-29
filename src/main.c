/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "draw.h"
#include "init.h"
#include "title.h"
#include "input.h"
#include "main.h"

/* Externs. */
App app;
Highscores highscores;
Stage stage;

/* Functions. */
static void capFrameRate(long *then, float *remainder);

/* ----- Main Program ----- */

/* Main initializer. */
int main(int argc, char *argv[])
{
    long then; // Store integer with a larger range than int
    float remainder; // Accounts for any fractional time

    // Redirect console messages to "consolelog.txt"
    freopen("consolelog.txt", "w", stdout);

	printf("----------------------------------------------------------------------------------------------------\n");

	printf("voidFighter - New Session\n");
	
	printf("Copyright (C) 2023-2024 Asephri. All rights reserved.\n");

	printf("consolelog.txt\n");
	
	printf("----------------------------------------------------------------------------------------------------\n");

    // Creating a memory block for the application
    memset(&app, 0, sizeof(App));

    // Initializing SDL.
    initSDL();

    // Register cleanup function to be called at exit
    atexit(cleanup);

    // Initializing the Game.
    initGame();

    // Initializing the scene.
    initTitle();

    // Frame rate control variables.
    then = SDL_GetTicks();
    remainder = 0;

    /* Main game loop */
    while (1)
    {
        // Prepare the scene for rendering.
        prepareScene();

        // Process user input.
        doInput();

        // Update game logic.
        app.delegate.logic();

        // Draw the game.
        app.delegate.draw();

        // Present the scene for display.
        presentScene();

        // Cap the frame rate to achieve a stable frame rate.
        capFrameRate(&then, &remainder);
    }

    return 0;
}

/* Frame Rate Cap. */
static void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;

    // Set the desired time interval between frames (60 FPS is approximately 16 milliseconds per frame).
    wait = 16 + *remainder;

    // Remove the fractional part of the remainder to ensure accurate time measurement.
    *remainder -= (int)*remainder;

    // Calculate the time elapsed since the last frame.
    frameTime = SDL_GetTicks() - *then;

    // Calculate the remaining time to wait before the next frame.
    wait -= frameTime;

    // Ensure there's a minimum wait time of 1 millisecond to avoid busy-waiting.
    if (wait < 1)
    {
        wait = 1;
    }

    // Delay execution for the calculated wait time.
    SDL_Delay(wait);

    // Increment the remainder to account for fractional time for the next frame.
    *remainder += 0.667;

    // Update the reference time for the next frame.
    *then = SDL_GetTicks();
}