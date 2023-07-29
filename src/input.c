/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "input.h"

/* Externs. */
extern App app;

/* ----- User Input ------ */

// Handle Arrow Key up input.
static void doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) // Check if it's not a repeat.
    {
        app.keyboard[event->keysym.scancode] = 0; // Set the key state to released (0).
        printf("// Console message. Key released: %d\n", event->keysym.scancode);
    }
}

// Handle Arrow Key Down input.
static void doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) // Check if it's not a repeat.
    {
        app.keyboard[event->keysym.scancode] = 1; // Set the key state to pressed (1).
        printf("// Console message. Key pressed: %d\n", event->keysym.scancode);
    }
}

// User Movement and Controls Input.
void doInput(void)
{
    SDL_Event event;

    memset(app.inputText, '\0', MAX_LINE_LENGTH); // Clearing memory for input Text.

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                exit(0); // Quit the application when the user clicks the close button.
                break;

            case SDL_KEYDOWN:
                doKeyDown(&event.key); // Handle the key press event.
                break;

            case SDL_KEYUP:
                doKeyUp(&event.key); // Handle the key release event.
                break;

            case SDL_TEXTINPUT:
                STRNCPY(app.inputText, event.text.text, MAX_LINE_LENGTH); // Copy the text input into the app's inputText variable.
                break;

            default:
                break;
        }
    }
}