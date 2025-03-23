/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include <SDL2/SDL_image.h>

#include "common.h"
#include "draw.h"

extern App app;

void prepareScene(void)
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 255, 255);
    SDL_RenderClear(app.renderer);

    //printf("Scene prepared.\n");
}

void presentScene(void)
{
    SDL_RenderPresent(app.renderer);

    //printf("Scene presented.\n");
}

static void addTextureToCache(char *name, SDL_Texture *sdlTexture)
{
    Texture *texture = malloc(sizeof(Texture));
    memset(texture, 0, sizeof(Texture));
    STRNCPY(texture->name, name, MAX_NAME_LENGTH);
    texture->texture = sdlTexture;
    texture->next = NULL;

    if (app.textureTail == NULL)
    {
        app.textureHead.next = texture;
    }
    else
    {
        app.textureTail->next = texture;
    }

    app.textureTail = texture;

    printf("Texture '%s' added to the cache.\n", name);
}

static SDL_Texture *getTexture(char *name)
{
    Texture *t;
    for (t = app.textureHead.next; t != NULL; t = t->next)
    {
        if (strcmp(t->name, name) == 0)
        {
            return t->texture;
        }
    }

    return NULL;
}

SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;

    texture = getTexture(filename);

    if (texture == NULL)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
        texture = IMG_LoadTexture(app.renderer, filename);.
        if (texture == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s\n", SDL_GetError());
            return NULL;
        }
        addTextureToCache(filename, texture);
    }

    return texture;
}

void blit(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = 64;
    dest.h = 64;

    SDL_Rect dstrect;
    dstrect.w = 16;
    dstrect.h = 16;
    SDL_QueryTexture(texture, NULL, NULL, &dstrect.w, &dstrect.h);
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);

    //printf("Texture rendered at (%d, %d).\n", x, y);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(app.renderer, texture, src, &dest);

    //printf("Texture rect rendered at (%d, %d).\n", x, y);
}
