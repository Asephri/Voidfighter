/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include "common.h"
#include "draw.h"
#include "text.h"

static SDL_Texture *fontTexture;
static char drawTextBuffer[MAX_LINE_LENGTH];
static int align;

void initFonts(void)
{
    fontTexture = loadTexture("gfx/font.png");
    if (fontTexture == NULL)
    {
        fprintf(stderr, "// SDL Error: Failed to load font texture: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    printf("// Font initialized.\n");
}

void drawText(int x, int y, int r, int g, int b, char *format, ...)
{
    int i, len, c;
    SDL_Rect rect;
    va_list args;

    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    switch (align)
    {
        case TEXT_RIGHT:
            x -= (len * GLYPH_WIDTH);
            break;

        case TEXT_CENTER:
            x -= (len * GLYPH_WIDTH) / 2;
            break;
    }

    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    SDL_SetTextureColorMod(fontTexture, r, g, b);

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

void drawTextPOSITION(int x, int y, int r, int g, int b, int align, char *format, ...)
{
    int i, len, c;
    SDL_Rect rect;
    va_list args;

    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    switch (align)
    {
        case TEXT_RIGHT:
            x -= (len * GLYPH_WIDTH);
            break;

        case TEXT_CENTER:
            x -= (len * GLYPH_WIDTH) / 2;
            break;
    }

    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    SDL_SetTextureColorMod(fontTexture, r, g, b);

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
