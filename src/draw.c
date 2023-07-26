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

/* ----- Scene Creation ----- */

/* Staging the Renderer. */
void prepareScene(void)
{
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 255, 255);
	SDL_RenderClear(app.renderer);
}

/* Presenting the scene. */
void presentScene(void)
{
	SDL_RenderPresent(app.renderer);
}
/* ---------- */

/* adding Texture to cache. */
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
}

/* Getting the texture. */
static SDL_Texture *getTexture(char *name)
{
	Texture *t;

	for (t = app.textureHead.next ; t != NULL ; t = t->next)
	{
		if (strcmp(t->name, name) == 0)

		{
			return t->texture;
		}
	}

	return NULL;
}
/* ---------- */

// SDL_Texture loading function.
SDL_Texture *loadTexture(char *filename)
{
	SDL_Texture *texture;

	texture = getTexture(filename);

	if (texture == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
		texture = IMG_LoadTexture(app.renderer, filename);
		addTextureToCache(filename, texture);
	}

	return texture;
}

/* ---------- */

/* Creating the Block Information Transfer for texuring. */
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
}
/* ---------- */



/* Using the source texture for information. */
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x,int y)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;

	SDL_RenderCopy(app.renderer, texture, src, &dest);
}
/* ---------- */