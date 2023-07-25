/* 
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "draw.h"
#include "text.h"

/* Variables. */
static SDL_Texture *fontTexture;
static char         drawTextBuffer[MAX_LINE_LENGTH];

/* ----- Text Creation ----- */

/* Intialising Fonts. */
void initFonts(void)
{
	fontTexture = loadTexture("gfx/font.png");
}

/* Drawing Text to screen. */
void drawText(int x, int y, int r, int g, int b, char *format, ...)
{
    int i, len, c;
    SDL_Rect rect; // Create drawing region for text.
    va_list args;

    // Allocating Memory and clearing the buffer.
    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    // Using C's varargs functions to format the string.
    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    // Assinging text the width and height of the texture.
    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    // Set the colour to have the text drawn with.
    SDL_SetTextureColorMod(fontTexture, r, g, b);

    // Loop through each character in the string.
	for (i = 0; i < len; i++)
	{
		c = drawTextBuffer[i];

		if (c >= ' ' && c <= 'Z')
		{
			rect.x = (c - ' ') * GLYPH_WIDTH;

			blitRect(fontTexture, &rect, x, y);

			x += GLYPH_WIDTH;
		}
	}
}
/* ----------- */