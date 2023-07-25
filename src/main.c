/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"
#include "sound.h"
#include "text.h"
#include "stage.h"

/* Externs. */
App app;
Stage stage;

/* Functions. */
static void capFrameRate(long *then, float *remainder);

/* ----- Main Program ----- */

/* Main intialiser. */
int main(int argc, char *argv[])
{
	long then; // Store integer with larger range then int
	float remainder; // Accounts for any fractional time
	// Creating memory block for the application
	memset(&app, 0, sizeof(App));
    // intialising SDL.
	initSDL();
    // Cleaning up all functions.
	atexit(cleanup);
	// Intialising audio.
	initSounds();
	// Intialising Fonts.
	initFonts();
	// Intialising the scene.
	initStage();
	// Frame rate.
	then = SDL_GetTicks();
	remainder = 0;

    /* while loop */
	while (1)
	{   
        // Calling the prepare scene function.
		prepareScene();
        // Calling the do input function.
		doInput();
		// Calling logic.
		app.delegate.logic();
		// Calling drawing.
		app.delegate.draw();
		// Calling the present scene function.
		presentScene();
        // fFrame rate cap function.
		capFrameRate(&then, &remainder);
	}

	return 0;
}
/* ---------- */

/* Frame Rate Cap. */
static void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;  // Set the initial wait time between frames (60 FPS is approximately 0.0167 seconds per frame (1 second divided by 60), which is approximately 16 milliseconds per frame.)

    *remainder -= (int)*remainder;  // Remove the fractional part of remainder

    frameTime = SDL_GetTicks() - *then;  // Calculate the time elapsed since the last frame

    wait -= frameTime;  // Calculate the remaining time to wait before the next frame

    if (wait < 1)
    {
        wait = 1;  // Set a minimum wait time of 1 millisecond
    }

    SDL_Delay(wait);  // Delay execution for the calculated wait time

    *remainder += 0.667;  // Increment remainder to account for fractional time

    *then = SDL_GetTicks();  // Update the reference time for the next frame
}
/* ---------- */