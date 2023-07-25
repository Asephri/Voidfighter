/* 
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

#include "defs.h"

/* Typedefines. */
typedef struct Entity Entity;	// inheritance
typedef struct Explosion Explosion;
typedef struct Debris Debris;
typedef struct trail trail;
typedef struct fire fire;
/* ---------- */

/* logic delegation structure. */
typedef struct structs
{
	void(*logic)(void);
	void(*draw)(void);
} Delegate; // handling game logic and drawing in main loop.
/* ---------- */

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window   *window;
	Delegate delegate;
	int keyboard[MAX_KEYBOARD_KEYS]; //holds the state of all keys on the keyboard.
} App;
/* ---------- */

/* Generic Entity */
struct Entity {
	float x; //float holds real numbers as variables.
	float y;
	int w;
	int h;
	float dx;
	float dy;
	int health;	// Entity health used for entity deletion.
	int reload; // tracks players ability to reload.
	int side; // uses _SIDE declaration in defs.
	int alpha;
	SDL_Texture *texture;
	Entity *next; // used for linking lists of entities.
};
/* ---------- */

/* Explosion Effect */
struct Explosion
{
	float      x;
	float      y;
	float      dx;
	float      dy;
	int        r, g, b, a;
	Explosion *next;
};
/* ---------- */

/* Debris Effect. */
struct Debris
{
	float        x;
	float        y;
	float        dx;
	float        dy;
	SDL_Rect     rect;
	SDL_Texture *texture;
	int          life;
	Debris      *next;
};
/* ---------- */

/* Trail Effect. */
struct trail
{
	float      x;
	float      y;
	float      dx;
	float      dy;
	int        r, g, b, a;
	trail *next;
};
/* ---------- */

/* Fire Effect. */
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
/* ---------- */

/* Stage Structures for entity Tracking */

typedef struct
{
	Entity    fighterHead, *fighterTail;
	Entity    bulletHead, *bulletTail;
	Explosion explosionHead, *explosionTail;
	Debris    debrisHead, *debrisTail;
	trail trailHead, *trailTail;
	fire fireHead, *fireTail;
	int score; // Holds the current score
} Stage;
/* ---------- */

/* Star Effect */
typedef struct
{
	int x;
	int y;
	int speed;
} Star;
/* ---------- */