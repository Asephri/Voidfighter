/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

typedef struct Entity Entity;
typedef struct Explosion Explosion;
typedef struct Debris Debris;
typedef struct trail trail;
typedef struct fire fire;
typedef struct Texture Texture;

typedef struct
{
	void(*logic)(void);
	void(*draw)(void);
} Delegate;

struct Texture
{
	char name[MAX_NAME_LENGTH];
	SDL_Texture *texture;
	Texture *next;
};

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window *window;
	Delegate delegate;
	int keyboard[MAX_KEYBOARD_KEYS];
	Texture textureHead, *textureTail;
	char inputText[MAX_LINE_LENGTH];
} App;

struct Entity 
{
	float x;
	float y;
	int w;
	int h;
	float dx;
	float dy;
	int health;
	int reload;
	int side;
	int alpha;
	SDL_Texture *texture;
	Entity *next;
};

struct Explosion
{
	float x;
	float y;
	float dx;
	float dy;
	int r, g, b, a;
	Explosion *next;
};

struct Debris
{
	float x;
	float y;
	float dx;
	float dy;
	SDL_Rect rect;
	SDL_Texture *texture;
	int life;
	Debris *next;
};

struct trail
{
	float x;
	float y;
	float dx;
	float dy;
	int r, g, b, a;
	trail *next;
};

struct fire
{
	float        x;
	float        y;
	float        dx;
	float        dy;
	SDL_Rect     rect;
	SDL_Texture *texture;
	int          life;
	fire      *next;
};

typedef struct
{
	Entity fighterHead, *fighterTail;
	Entity bulletHead, *bulletTail;
	Explosion explosionHead, *explosionTail;
	Debris debrisHead, *debrisTail;
	trail trailHead, *trailTail;
	fire fireHead, *fireTail;
	Entity pointsHead, *pointsTail;

	int score;
} Stage;

typedef struct
{
	int x;
	int y;
	int speed;
} Star;

typedef struct
{
	char name[MAX_SCORE_NAME_LENGTH];
	int recent;
	int score;
} Highscore;

typedef struct
{
	Highscore highscore[NUM_HIGHSCORES];
} Highscores;
