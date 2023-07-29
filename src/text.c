/* 
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers. */
#include "common.h"
#include "draw.h"
#include "text.h"

/* Variables. */
static SDL_Texture *fontTexture;
static char drawTextBuffer[MAX_LINE_LENGTH];
static int align;

/* ----- Text Creation ----- */

// Initialize fonts by loading the font texture.
void initFonts(void)
{
    fontTexture = loadTexture("gfx/font.png");
    if (fontTexture == NULL)
    {
        fprintf(stderr, "// SDL Error: Failed to load font texture: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Console message.
    printf("// Font initialized.\n");
}

// Draw text on the screen at the specified position with the given color.
void drawText(int x, int y, int r, int g, int b, char *format, ...)
{
    int i, len, c;
    SDL_Rect rect; // Create a drawing region for text.
    va_list args;

    // Allocate memory and clear the buffer.
    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    // Use C's varargs functions to format the string.
    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    // Control text placement based on the alignment setting.
    switch (align)
    {
        case TEXT_RIGHT:
            x -= (len * GLYPH_WIDTH);
            break;

        case TEXT_CENTER:
            x -= (len * GLYPH_WIDTH) / 2;
            break;
    }

    // Assign text the width and height of the texture.
    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    // Set the color to draw the text.
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

// Draw text on the screen at the specified position with the given color and alignment.
void drawTextPOSITION(int x, int y, int r, int g, int b, int align, char *format, ...)
{
    int i, len, c;
    SDL_Rect rect; // Create a drawing region for text.
    va_list args;

    // Allocate memory and clear the buffer.
    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    // Use C's varargs functions to format the string.
    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    // Control text placement based on the alignment setting.
    switch (align)
    {
        case TEXT_RIGHT:
            x -= (len * GLYPH_WIDTH);
            break;

        case TEXT_CENTER:
            x -= (len * GLYPH_WIDTH) / 2;
            break;
    }

    // Assign text the width and height of the texture.
    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    // Set the color to draw the text.
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
