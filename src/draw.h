/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

void prepareScene(void);
void presentScene(void);
SDL_Texture *loadTexture(char *filename);
void blit(SDL_Texture *texture, int x, int y);
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
