/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Libraries. */
#include <SDL2/SDL_image.h>

/* Headers. */
#include "common.h"
#include "draw.h"

/* Externs. */
extern App app;

/* Scene Creation */

/* Prepare the scene by setting the background color. */
void prepareScene(void)
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 255, 255); // Set the background color to blue (R,G,B,Alpha)
    SDL_RenderClear(app.renderer); // Clear the renderer with the specified color.
    
    // Console message
    //printf("Scene prepared.\n");
}

/* Present the scene to the screen by rendering the contents. */
void presentScene(void)
{
    SDL_RenderPresent(app.renderer); // Update the screen with the rendered content.
    
    // Console message
    //printf("Scene presented.\n");
}

/* Add a loaded texture to the texture cache. */
static void addTextureToCache(char *name, SDL_Texture *sdlTexture)
{
    Texture *texture = malloc(sizeof(Texture));
    memset(texture, 0, sizeof(Texture));
    STRNCPY(texture->name, name, MAX_NAME_LENGTH);
    texture->texture = sdlTexture;
    texture->next = NULL; // Initialize the next pointer to NULL.

    if (app.textureTail == NULL)
    {
        app.textureHead.next = texture;
    }
    else
    {
        app.textureTail->next = texture;
    }

    app.textureTail = texture;
    
    // Console message
    printf("Texture '%s' added to the cache.\n", name);
}

/* Get a texture from the texture cache. */
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

/* Load a texture from a file, either by retrieving it from the cache or loading it from the file system. */
SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;

    texture = getTexture(filename);

    if (texture == NULL)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename); // Log loading message.
        texture = IMG_LoadTexture(app.renderer, filename); // Load the texture from the file system.
        if (texture == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s\n", SDL_GetError()); // Log error message if loading failed.
            return NULL;
        }
        addTextureToCache(filename, texture); // Add the loaded texture to the cache.
    }

    return texture;
}

/* Render a texture on the screen at the specified position. */
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
    
    // Console message
    //printf("Texture rendered at (%d, %d).\n", x, y);
}

/* Render a portion of a texture (defined by the src rectangle) on the screen at the specified position. */
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(app.renderer, texture, src, &dest);
    
    // Console message
    //printf("Texture rect rendered at (%d, %d).\n", x, y);
}
