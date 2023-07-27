/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Functions. */
void prepareScene(void);
void presentScene(void);
SDL_Texture *loadTexture(char *filename);
void blit(SDL_Texture *texture, int x, int y);
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
/* ---------- */
