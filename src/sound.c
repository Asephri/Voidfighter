/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
 */

 /* Libraries. */
#include <SDL2/SDL_mixer.h>

/* Headers. */
#include "common.h"
#include "sound.h"

/* Functions. */
static void loadSounds(void);

/* Variables */
static Mix_Chunk *sounds[SND_MAX];
static Mix_Music *music;

/* ----- Creating the Audio System ----- */

/* Intialising sound effects. */
void initSounds(void)
{
    memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

    music = NULL;

    loadSounds();
}
/* ---------- */

/* Loading music. */
void loadMusic(char *filename)
{
    if (music != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }

    music = Mix_LoadMUS(filename);
}
/* ---------- */

/* Playing music. */
void playMusic(int loop)
{
    Mix_PlayMusic(music, (loop) ? -1 : 0);
}
/* ---------- */

/* Playing sounds. */
void playSound(int id, int channel)
{
    Mix_PlayChannel(channel, sounds[id], 0);
}
/* ---------- */

/* Loading sound channels. */
static void loadSounds(void)
{
    sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sound/voidfighter - Track 02 (PlayerFire).ogg");
    sounds[SND_ENEMY_FIRE] = Mix_LoadWAV("sound/voidfighter - Track 03 (EnemyFire).ogg");
    sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sound/voidfighter - Track 04 (Explosion).ogg");
    sounds[SND_ENEMY_DIE] = Mix_LoadWAV("sound/voidfighter - Track 05 (EnemyDeath).ogg");
    sounds[SND_POINTS] = Mix_LoadWAV("sound/voidfighter - Track 06 (points).ogg");
}   
/* ---------- */