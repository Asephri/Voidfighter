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

// Initialize sound effects.
void initSounds(void)
{
    memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

    music = NULL;

    loadSounds();

    printf("// Audio system initialized.\n");
}

/* ----- Loading Music ----- */

// Load music from file.
void loadMusic(char *filename)
{
    if (music != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }

    music = Mix_LoadMUS(filename);
    if (music == NULL)
    {
        fprintf(stderr, "// SDL Mixer Error: Failed to load music '%s': %s\n", filename, Mix_GetError());
        return;
    }

    printf("// Music loaded: %s\n", filename);
}

/* ----- Playing Music ----- */

// Play music with the specified loop count.
void playMusic(int loop)
{
    Mix_PlayMusic(music, (loop) ? -1 : 0);
    if (music == NULL)
    {
        fprintf(stderr, "// SDL Mixer Error: Failed to play music: %s\n", Mix_GetError());
        return;
    }

    printf("// Music playing.\n");
}

/* ----- Playing Sound Effects ----- */

// Play a sound effect on the specified channel.
void playSound(int id, int channel)
{
    Mix_PlayChannel(channel, sounds[id], 0);
    if (sounds[id] == NULL)
    {
        fprintf(stderr, "// SDL Mixer Error: Failed to play sound effect %d: %s\n", id, Mix_GetError());
        return;
    }

    printf("// Sound effect %d playing on channel %d.\n", id, channel);
}

/* ----- Loading Sound Channels ----- */

// Load sound channels from file.
static void loadSounds(void)
{
    sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sound/voidfighter - Track 02 (PlayerFire).ogg");
    sounds[SND_ENEMY_FIRE] = Mix_LoadWAV("sound/voidfighter - Track 03 (EnemyFire).ogg");
    sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sound/voidfighter - Track 04 (Explosion).ogg");
    sounds[SND_ENEMY_DIE] = Mix_LoadWAV("sound/voidfighter - Track 05 (EnemyDeath).ogg");
    sounds[SND_POINTS] = Mix_LoadWAV("sound/voidfighter - Track 06 (points).ogg");
    
    for (int i = 0; i < SND_MAX; i++)
    {
        if (sounds[i] == NULL)
        {
            fprintf(stderr, "// SDL Mixer Error: Failed to load sound effect %d: %s\n", i, Mix_GetError());
            continue;
        }

        printf("// Sound effect %d loaded.\n", i);
    }
}
