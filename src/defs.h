/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* ----- Preventing Loop recursion ----- */
#ifndef DEFS_H
#define DEFS_H

/* ----- Text Defines ----- */
/* Application Name */
#define PROJECT_NAME "Voidfighter"

// Text width and height.
#define GLYPH_HEIGHT 28
#define GLYPH_WIDTH  18

/* Highscores */
#define NUM_HIGHSCORES 5

/* Text Array Limit.*/
#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'
#define MAX_NAME_LENGTH 32
/* Max size for a character string. */
#define MAX_LINE_LENGTH 1024

/* Hud Resolution. */
#define HUD_WIDTH 1024
#define HUD_HEIGHT 128

/* Hudscreen Bounds. */
#define HUDSCREEN_WIDTH 928
#define HUDSCREEN_HEIGHT 672
#define HUDSCREEN_X 32
#define HUDSCREEN_Y 32

/* Screen Resolution. */
#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768 + HUD_HEIGHT
#define SCREEN_BOUNDS 16

/* Hud Text. */
#define HUD_HEALTH_POS_HEIGHT SCREEN_HEIGHT-35
#define HUD_HEALTH_POS_WIDTH 10
#define HUD_SCORE_POS_HEIGHT SCREEN_HEIGHT-75
#define HUD_SCORE_POS_WIDTH 10
#define HUD_HIGHSCORE_POS_HEIGHT SCREEN_HEIGHT-115
#define HUD_HIGHSCORE_POS_WIDTH 10

/* Bullet CoolDown Timer on Hud. */
#define PLAYER_FIRE_COOLDOWN 30

/* Collision Detection. */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Alien defines. */
#define ENEMY_BULLET_SPEED 18
#define ENEMY_SPEED 2
#define ENEMY_SPAWN_TIME 30
/* Player Defines. */
#define PLAYER_HEALTH 25
#define PLAYER_SPEED 4
#define PLAYER_BULLET_SPEED 8
#define PLAYER_RELOAD_TIME 16
#define PLAYER_PUSHBACK 1
// Player firing
#define AMMUNITION 16

// Max keyboard keys
#define MAX_KEYBOARD_KEYS 350

/* Entity Team behavior Define. */
#define SIDE_PLAYER 0
#define SIDE_ENEMY 1

/* Frames per second. */
#define FPS 60

/* Starfield Defines. */
#define MAX_STARS 500

/* Amount of Sound channels available. */
#define MAX_SND_CHANNELS 8

enum ChannelType
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ENEMY_FIRE,
	CH_POINTS
};

enum SoundType
{
	SND_PLAYER_FIRE,
	SND_ENEMY_FIRE,
	SND_PLAYER_DIE,
	SND_ENEMY_DIE,
	SND_POINTS,
	SND_MAX
};

#endif // DEFS_H
