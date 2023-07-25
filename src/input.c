/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "input.h"

/* Externs. */
extern App app;

/* ----- User Input ------ */

/* Arrow Key up input */
static void doKeyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) //if its not a repeat.
	{
		app.keyboard[event->keysym.scancode] = 0;
	}
}
/* ---------- */

/* Arrow Key Down input. */
static void doKeyDown(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) //if repeat is true!
	{
		app.keyboard[event->keysym.scancode] = 1;
	}
}
/* ---------- */

/* User Movement and Controls Input. */
void doInput(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				doKeyDown(&event.key);
				break;

			case SDL_KEYUP:
				doKeyUp(&event.key);
				break;

			default:
				break;
		}
	}
}
/* ---------- */