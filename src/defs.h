/*
Copyright (C) 2023-2024 Asephri.net. All rights reserved.
*/

#define PROJECT_NAME "Voidfighter"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


#define NUM_HIGHSCORES 5

#define STRNCPY(dest, src, n) \
strncpy(dest, src, n); \
dest[n - 1] = '\0'

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 736 + HUD_HEIGHT
#define MAX_SCORE_NAME_LENGTH 8
#define MAX_NAME_LENGTH 8
#define MAX_LINE_LENGTH 1024

#define FPS 60

#define PLAYER_HEALTH 25
#define PLAYER_SPEED 4
#define PLAYER_BULLET_SPEED 8

#define MAX_STARS 500

#define MAX_KEYBOARD_KEYS 350

#define SIDE_PLAYER 0
#define SIDE_ENEMY 1

#define GLYPH_HEIGHT 28
#define GLYPH_WIDTH  18

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

enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT	
};

#define HUD_WIDTH 1024
#define HUD_HEIGHT 32

#define HUDSCREEN_WIDTH 928
#define HUDSCREEN_HEIGHT 672
#define HUDSCREEN_X 32
#define HUDSCREEN_Y 32
#define SCREEN_BOUNDS 16

#define HUD_HEALTH_POS_HEIGHT SCREEN_HEIGHT-28
#define HUD_HEALTH_POS_WIDTH 85
#define HUD_SCORE_POS_HEIGHT SCREEN_HEIGHT-28
#define HUD_SCORE_POS_WIDTH 341
#define HUD_HIGHSCORE_POS_HEIGHT SCREEN_HEIGHT-28
#define HUD_HIGHSCORE_POS_WIDTH 682

#define PLAYER_FIRE_COOLDOWN 30
#define PLAYER_RELOAD_TIME 16
#define PLAYER_PUSHBACK 1

#define ENEMY_BULLET_SPEED 18
#define ENEMY_SPEED 2
#define ENEMY_SPAWN_TIME 30

#define AMMUNITION 16
