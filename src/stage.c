/*
Copyright (C) 2023-2024 Asephri. All rights reserved.
*/

/* Headers */
#include "common.h"
#include "draw.h"
#include "sound.h"
#include "stage.h"
#include "text.h"
#include "util.h"

/* Externs. */
extern App app;
extern Stage stage;

/* Functions. */
static void logic(void);
static void draw(void);
static void initPlayer(void);
static void fireBullet(void);
static void doPlayer(void);
static void doFighters(void);
static void doBullets(void);
static void drawFighters(void);
static void drawBullets(void);
static void spawnEnemies(void);
static int bulletHitFighter(Entity *b);
static void doEnemies(void);
static void fireEnemyBullet(Entity *e);
static void clipPlayer(void);
static void clipEnemies(void);
static void checkPlayerEnemyCollisions(void);
static void resetStage(void);
static void initStars(void);
static void doStars(void);
static void drawExplosions(void);
static void doExplosions(void);
static void addExplosions(int x, int y, int num);
static void addDebris(Entity *e);
static void doDebris(void);
static void drawDebris(void);
static void drawtrails(void);
static void dotrails(void);
static void addtrails(int x, int y, int num);
static void drawfire(void);
static void dofire(void);
static void addfire(Entity *e);
static void drawHud(void);

/* Variables */
static Entity *player;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *EnemyBulletTexture;
static SDL_Texture *playerTexture;
static SDL_Texture *background;
static SDL_Texture *explosionTexture;
static SDL_Texture *trailTexture;
static SDL_Texture *fireTexture;
static SDL_Texture *hudTexture;
static SDL_Texture *hudeffectsTexture;
static SDL_Texture* hudscreenTexture;
static int enemySpawnTimer;
static int stageResetTimer;
static int backgroundX;
static int hudX;
static int hudeffectsX;
static int hudscreenX;
static Star stars[MAX_STARS];
static int highscore;
static int HUD_HEALTH_BUFFER[3];

/* ----- Stage Creation ----- */

/* Intialising the Stage. */
void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    // Initialize the stage structure
    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;
    stage.trailTail = &stage.trailHead;
    stage.fireTail = &stage.fireHead;

    // Load textures
    bulletTexture = loadTexture("gfx/playerBullet.png");
    enemyTexture = loadTexture("gfx/enemy.png");
    EnemyBulletTexture = loadTexture("gfx/enemyBullet.png");
    playerTexture = loadTexture("gfx/player.png");
    background = loadTexture("gfx/background.png");
    explosionTexture = loadTexture("gfx/explosion.png");
    trailTexture = loadTexture("gfx/trail.png");
    fireTexture = loadTexture("gfx/fire.png");
    hudTexture = loadTexture("gfx/hud.png");
    hudeffectsTexture = loadTexture("gfx/hudeffects.png");
    hudscreenTexture = loadTexture("gfx/hudscreen.png");

    // Load Music
    loadMusic("music/voidfighter - Track 01 (deepspace-01).ogg");

    // Play music
    playMusic(1);

    // Reset the stage
    resetStage();
}
/* ---------- */

/* Resetting the Stage. */
static void resetStage(void)
{
    Entity *e;
    Explosion *ex;
    Debris *d;
    trail *tr;
    fire *f;

    // Clear existing entities
    while (stage.fighterHead.next)
    {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        free(e);
    }

    while (stage.bulletHead.next)
    {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    while (stage.explosionHead.next)
    {
        ex = stage.explosionHead.next;
        stage.explosionHead.next = ex->next;
        free(ex);
    }
    while (stage.debrisHead.next != NULL)
    {
        d = stage.debrisHead.next;
        stage.debrisHead.next = d->next;
        free(d);
    }
    while (stage.trailHead.next)
    {
        tr = stage.trailHead.next;
        stage.trailHead.next = tr->next;
        free(tr);
    }
    while (stage.fireHead.next)
    {
        f = stage.fireHead.next;
        stage.fireHead.next = f->next;
        free(f);
    }

    // Reset stage properties.
    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;
    stage.trailTail = &stage.trailHead;
    stage.fireTail = &stage.fireHead;

    // Reset score counter.
    stage.score = 0;
    // Initialize the player entity.
    initPlayer();
    // Initialize the star entities.
    initStars();

    // Reset timers.
    enemySpawnTimer = 0;
    stageResetTimer = FPS * 3;
}
/* ---------- */

/* ----- Creating The Player ----- */

/* Intialising the Player. */
static void initPlayer()
{
    // Allocate memory for the player entity.
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    // Set player properties.
    player->health = 25;
    player->x = 100;
    player->y = (SCREEN_HEIGHT - HUD_HEIGHT)  /2;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
    player->side = SIDE_PLAYER;
}
/* ---------- */

/* ----- Creating a Starfield Effect ----- */

/* Intialising Stars. */
static void initStars(void)
{
    int i;
    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % (SCREEN_HEIGHT - HUD_HEIGHT)  ;
        stars[i].speed = 1 + rand() % 8;
    }   
}
/* ---------- */

/* ----- Creating The Background ----- */

/* Background Behavour. */
static void doBackground(void)
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}
/* ---------- */

static void doHud(void)
{
    if (--hudX < -SCREEN_WIDTH)
    {
        hudX = 0;
    }
}
/* ---------- */

static void doHudEffects(void)
{
    if (--hudeffectsX < -SCREEN_WIDTH)
    {
        hudeffectsX = 0;
    }
}
/* ---------- */
static void doHudscreen(void)
{
    hudscreenX = 0;
}
/* ---------- */

/* Local file logic. */
static void logic(void)
{   
    doBackground();
	doStars();
	doPlayer();
	doEnemies();
	doFighters();
	doBullets();
	doExplosions();
	doDebris(); 
    dotrails();
    dofire();
    doHud();
    doHudEffects();
    doHudscreen();
	spawnEnemies();
	clipPlayer();
    clipEnemies();
	checkPlayerEnemyCollisions();

    if (player == NULL && --stageResetTimer <= 0)
    {
        resetStage();
    }
}
/* ---------- */

/* The Stars behaviour. */
static void doStars(void)
{
    int i;
    
    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        if (stars[i].x <0)
        {
            stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
}
/* ---------- */

/* User Play Controls. */
static void doPlayer(void)
{
    // Handle player actions.
    if (player != NULL)
    {
        player->dx = player->dy = 0;
        player->x = player->x - PLAYER_PUSHBACK;

        // Cooldown ticking
        if (player->reload > 0)
        {
            player->reload--;
        }

        if (app.keyboard[SDL_SCANCODE_UP])
        {
            player->dy = -PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_DOWN])
        {
            player->dy = PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_LEFT])
        {
            player->dx = -PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_RIGHT])
        {
            player->dx = PLAYER_SPEED + 2.5;
            addtrails((player->x - 1), player->y, 32);
        }

        if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0)
        {
            playSound(SND_PLAYER_FIRE, CH_PLAYER);
            fireBullet();

            // Set the cooldown to the specified value (100)
            player->reload = PLAYER_FIRE_COOLDOWN;
        }
    }
}
/* ---------- */

/* Player Bullet Entity. */
static void fireBullet(void)
{
    // Create a bullet entity and fire it.
    Entity *bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = player->x;
    bullet->y = player->y - bullet->h;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    bullet->side = SIDE_PLAYER;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    player->reload = PLAYER_RELOAD_TIME;
}

/* ----- Creating Enemies ----- */

/* Setting Enemy Firing Behaviour. */
static void doEnemies(void)
{
    // Handle enemy actions.

    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            fireEnemyBullet(e);
            playSound(SND_ENEMY_FIRE, CH_ENEMY_FIRE);
        }
    }
}
/* ---------- */

/* ----- Fighter Entities ----- */

/* Fighter Behaviour. */
static void fireEnemyBullet(Entity *e)
{
    // Create an enemy bullet and fire it towards the player.

    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = EnemyBulletTexture;
    bullet->side = SIDE_ENEMY;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ENEMY_BULLET_SPEED;
    bullet->dy *= ENEMY_BULLET_SPEED;

    e->reload = (rand() % FPS * 2);
}
/* ---------- */

static void doFighters(void)
{
    // Update fighter entities.

    Entity *e, *prev;

    prev = &stage.fighterHead;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (e != player && e->x < -e->w)
        {
            e->health = 0;
        }

        if (e->health == 0)
        {
            if (e == player)
            {
                player = NULL;
            }

            if (e == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        // Move enemies side by side on the y-axis.
        if (e != player)
        {
            e->dy = sin(SDL_GetTicks() / FPS / 2) * ENEMY_SPEED;
        }
        prev = e;
    }
}
/* ---------- */

/* ----- Bullet Creation ----- */

/* Bullet Behaviour. */
static void doBullets(void)
{
    // Update bullet entities.

    Entity *b, *prev;

    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        b->x += b->dx;
        b->y += b->dy;

        if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > (SCREEN_HEIGHT - HUD_HEIGHT)  )
        {
            if (b == stage.bulletTail)
            {
                stage.bulletTail = prev;
            }

            prev->next = b->next;
            free(b);
            b = prev;
        }

        prev = b;
    }
}
/* ---------- */

/* Adding Explosions. */
static void addExplosions(int x, int y, int num)
{
	Explosion *e;
	int        i;

	for (i = 0; i < num; i++)
	{
		e = malloc(sizeof(Explosion));
		memset(e, 0, sizeof(Explosion));
		stage.explosionTail->next = e;
		stage.explosionTail = e;

		e->x = x + (rand() % 32) - (rand() % 32);
		e->y = y + (rand() % 32) - (rand() % 32);
		e->dx = (rand() % 10) - (rand() % 10);
		e->dy = (rand() % 10) - (rand() % 10);

		e->dx /= 10;
		e->dy /= 10;

		switch (rand() % 4)
		{
			case 0:
				e->r = 255;
				break;

			case 1:
				e->r = 255;
				e->g = 128;
				break;

			case 2:
				e->r = 255;
				e->g = 255;
				break;

			default:
				e->r = 255;
				e->g = 255;
				e->b = 255;
				break;
		}

		e->a = rand() % FPS * 3;
	}
}
/* ---------- */

/* Bullet Collision Behaviour. */
static void addDebris(Entity *e)
{
	Debris *d;
	int     x, y, w, h;
	    
    w = e->w / 2;
	h = e->h / 2;

	for (y = 0; y <= h; y += h)
	{
		for (x = 0; x <= w; x += w)
		{
			d = malloc(sizeof(Debris));
		 	memset(d, 0, sizeof(Debris));
			stage.debrisTail->next = d;
		 	stage.debrisTail = d;
		  	d->x = e->x + e->w / 2;
		  	d->y = e->y + e->h / 2;
	    	d->dx = -5 - (rand() % 5);
            d->dy = -5 - (rand() % 6);
	    	d->life = FPS * 2;
	    	d->texture = e->texture;

		   	d->rect.x = x;
		  	d->rect.y = y;
		 	d->rect.w = w;
			d->rect.h = h;
        }
	}
}
/* ---------- */

/* Adding Trails. */
static void addtrails(int x, int y, int num)
{
    trail *e;
    int i;

    for (i = 0; i < num; i++)
    {
        if (i >= 1)
            break; // Maximum of 1 trails reached, exit the loop.

        e = malloc(sizeof(trail));
        memset(e, 0, sizeof(trail));
        stage.trailTail->next = e;
        stage.trailTail = e;

        e->x = x;
        e->y = y;
        e->dy = (rand() % 5) - (rand() % 5);

        e->dy /= 5;

        switch (rand() % 4)
        {
            case 0:
                e->r = 255;
                break;

            case 1:
                e->r = 255;
                e->g = 128;
                break;

            case 2:
                e->r = 255;
                e->g = 255;
                break;

            default:
                e->r = 255;
                e->g = 255;
                e->b = 255;
                break;
        }

        e->a = rand() % FPS * 1.85; // Increase the multiplier for faster fade out.
    }
}
/* ---------- */

/* Adding Fire. */
static void addfire(Entity *e)
{
    fire *f;
    int x, y, w, h;
    
    w = e->w / 2;
    h = e->h / 2;


    for (y = 0; y <= h; y += h)
    {

        for (x = 0; x <= w; x += w)
        {
            f = malloc(sizeof(fire));
            memset(f, 0, sizeof(fire));         
            stage.fireTail->next = f;       
            stage.fireTail = f;     
            f->x = e->x + e->w / 2;
            f->y = e->y + e->h / 2;  
		  	f->dx = -5 - (rand() % 5);
            f->dy = -(5 + (rand() % 16));        
            f->life = FPS * 2;     
            f->texture = e->texture;
            f->rect.x = x;
            f->rect.y = y;
            f->rect.w = w;
            f->rect.h = h;
        }
    }
}
/* ---------- */

/* Bullet Collision Behaviour. */
static int bulletHitFighter(Entity *b)
{
    // Check if a bullet hits a fighter.

    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
        {
            b->health = 0;
            e->health -= 1;

            addExplosions(e->x, e->y, 32);
            
            if(e == player)
            {
                playSound(SND_PLAYER_DIE, CH_PLAYER);
            }
            else
            {
                playSound(SND_ENEMY_DIE, CH_ANY);

                //award points for score.
                stage.score++;
                highscore = MAX(stage.score, highscore);
            }

            int i;

            for (i = 0; i <=2; i++)
	        { 
                addfire(e);
            }

            for (i = 0; i <=1; i++)
	        { 
                addDebris(e);
            }

            return 1;
        }
    }

    return 0;
}
/* ---------- */

/* Setting Enemy Spawning behaviour. */
static void spawnEnemies(void)
{
    // Spawn enemy entities.

    Entity *enemy;

    if (--enemySpawnTimer <= 0)
    {
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % (SCREEN_HEIGHT - HUD_HEIGHT)  ;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->dx = -(2 + (rand() % 4));
        enemy->dy = 0;  // Enemies start with no vertical movement.

        enemy->side = SIDE_ENEMY;
        enemy->health = 1;

        enemy->reload = FPS * (1 + (rand() % 3));

        enemySpawnTimer = ENEMY_SPAWN_TIME + (rand() % FPS);
    }
}
/* ---------- */

/* Creating Player Scene Bounds. */
static void clipPlayer(void)
{
    // Clip the player's position within the screen.

    if (player != NULL)
    {
        if (player->x < SCREEN_BOUNDS*2)
        {
            player->x = SCREEN_BOUNDS*2;
        }

        if (player->y < 0)
        {
            player->y = 0;
        }

        if (player->x > SCREEN_WIDTH / 2 - SCREEN_BOUNDS*4)
        {
            player->x = SCREEN_WIDTH / 2 - SCREEN_BOUNDS*4;
        }

        if (player->y > (HUDSCREEN_HEIGHT)  - player->h - SCREEN_BOUNDS)
        {
            player->y = (HUDSCREEN_HEIGHT)  - player->h - SCREEN_BOUNDS;
        }
        if (player->y < (64) - player->h - SCREEN_BOUNDS)
        {
            player->y = (64) - player->h - SCREEN_BOUNDS;
        }
    }
}
/* ---------- */

static void clipEnemies(void)
{
    // Clip the enemy positions within the screen.

    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e != player)
        {
            if (e->y < 0)
            {
                e->y = 0;
            }

            if (e->x > SCREEN_WIDTH - e->w - SCREEN_BOUNDS)
            {
                e->x = SCREEN_WIDTH - e->w - SCREEN_BOUNDS;
            }

            if (e->y > (HUDSCREEN_HEIGHT)   - e->h - SCREEN_BOUNDS)
            {
                e->y = (HUDSCREEN_HEIGHT)   - e->h - SCREEN_BOUNDS;
            }
            if (e->y < (64) - e->h - SCREEN_BOUNDS)
            {
                e->y = (64) - e->h - SCREEN_BOUNDS;
            }
        }
    }
}
/* ---------- */

/* Fighter Collision behaviour. */
static void checkPlayerEnemyCollisions(void)
{
    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e != player && player != NULL && collision(player->x, player->y, player->w, player->h, e->x, e->y, e->w, e->h))
        {
            player->health = 0;
            e->health = 0;

            int i;

            for (i = 0; i <=76.25; i++)
	        { 
                playSound(SND_PLAYER_DIE, CH_ANY);
                addExplosions(e->x, e->y, 3.663003663003663); //human reaction time on average.
            }
        }

    }
}
/* ---------- */

/* Explosion behaviour. */
static void doExplosions(void)
{
    Explosion *e, *prev;

    prev = &stage.explosionHead;

    for (e = stage.explosionHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;
        e->y -= e->dy;

        if (--e->a <= 0)
        {
            if (e == stage.explosionTail)
            {
                stage.explosionTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }
        prev = e;
    }
}
/* ---------- */

/* ----- Creating Debris ----- */

/* Debris Behaviour. */
static void doDebris(void)
{
	Debris *d, *prev;

	prev = &stage.debrisHead;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		d->x += d->dx;
		d->y += d->dy;

		d->dy += 0.25;
        d->dx -= 0.35;

		if (--d->life <= 0)
		{
			if (d == stage.debrisTail)
			{
				stage.debrisTail = prev;
			}

			prev->next = d->next;
			free(d);
			d = prev;
		}

		prev = d;
	}
}
/* ---------- */

/* ------ Creating Trails ------ */

/* Trail behaviour. */
static void dotrails(void)
{
    trail *e, *prev;

    prev = &stage.trailHead;

    for (e = stage.trailHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;
        e->y -= e->dy;

        if (--e->a <= 0)
        {
            if (e == stage.trailTail)
            {
                stage.trailTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }
        prev = e;
    }
}
/* ---------- */

/* ----- Creating Fire ------ */

/* Fire Behaviour. */
static void dofire(void)
{
	fire *f, *prev;

	prev = &stage.fireHead;

	for (f = stage.fireHead.next; f != NULL; f = f->next)
	{
		f->x += f->dx;
		f->y += f->dy;

		f->dy += 0.25;
        f->dx -= 0.35;

		if (--f->life <= 0)
		{
			if (f == stage.fireTail)
			{
				stage.fireTail = prev;
			}

			prev->next = f->next;
			free(f);
			f = prev;
		}

		prev = f;
	}
}
/* ---------- */

/* ----------- */

/* Rendering Fighters. */
static void drawFighters(void)
{
	Entity *e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		blit(e->texture, e->x, e->y);
	}
}
/* ---------- */

/* Rendering Fighters. */
static void drawBullets(void)
{
    // Draw the bullet entities.

    Entity *b;

    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        blit(b->texture, b->x, b->y);
    }
}
/* ---------- */

/* Rendering the Stars. */
static void drawStars(void)
{
	int i, c;

	for (i = 0; i < MAX_STARS; i++)
	{
		c = 32 * stars[i].speed;

		SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}
/* ---------- */

static void drawBackground(void)
{
	SDL_Rect dest;
	int      x;

	for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
	{
		dest.x = x;
		dest.y = 0;
		dest.w = SCREEN_WIDTH;
		dest.h = (SCREEN_HEIGHT - HUD_HEIGHT)  ;

		SDL_RenderCopy(app.renderer, background, NULL, &dest);
	}
}
/* ---------- */

/* Rendering the Hud. */
static void drawHudX(void)
{
	SDL_Rect dest;
	dest.x = 0;
	dest.y = SCREEN_HEIGHT-HUD_HEIGHT;
    dest.w = HUD_WIDTH;
	dest.h = HUD_HEIGHT;
	SDL_RenderCopy(app.renderer, hudTexture, NULL, &dest);
}
/* ---------- */

/* Rendering the Hud Screen. */
static void drawHudscreen(void)
{
	SDL_Rect dest;
	int      x;
    x = hudscreenX;
	dest.x = x;
	dest.y = 0;
	dest.w = SCREEN_WIDTH;
	dest.h = SCREEN_HEIGHT-HUD_HEIGHT;

	SDL_RenderCopy(app.renderer, hudscreenTexture, NULL, &dest);
}
/* ---------- */

/* Rendering the Hud. */
static void drawHudeffectX(void)
{
	SDL_Rect dest;
	dest.x = 0;
	dest.y = SCREEN_HEIGHT-HUD_HEIGHT;
    dest.w = 320;
	dest.h = 128;
	SDL_RenderCopy(app.renderer, hudeffectsTexture, NULL, &dest);
}
/* ---------- */

static void drawDebris(void)
{
	Debris *d;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		blitRect(d->texture, &d->rect, d->x, d->y);
	}
}
/* ---------- */

/* Rendering Explosions. */
static void drawExplosions(void)
{
	Explosion *e;

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

	for (e = stage.explosionHead.next; e != NULL; e = e->next)
	{
		SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
		SDL_SetTextureAlphaMod(explosionTexture, e->a);

		blit(explosionTexture, e->x, e->y);
	}

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}
/* ---------- */

static void drawtrails(void)
{
	trail *e;

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(trailTexture, SDL_BLENDMODE_ADD);

	for (e = stage.trailHead.next; e != NULL; e = e->next)
	{
		SDL_SetTextureColorMod(trailTexture, e->r, e->g, e->b);
		SDL_SetTextureAlphaMod(trailTexture, e->a);

		blit(trailTexture, e->x, e->y);
	}

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}
/* ---------- */

/* Rendering Fire. */
static void drawfire(void)
{
	fire *f;

	for (f = stage.fireHead.next; f != NULL; f = f->next)
	{
		blitRect(fireTexture, &f->rect, f->x, f->y);
	}
}
/* ---------- */

/* HUD Text rendering. */
static void drawHud(void)
{
    drawText(HUD_SCORE_POS_WIDTH, HUD_SCORE_POS_HEIGHT, 255, 255, 255, "SCORE: %03d", stage.score);

    // Hud health
    int HUDHEALTH;
    if (player == NULL)
    {
        HUDHEALTH = 0;
    }
    else
    {
        HUDHEALTH = player->health;
    }

    // Update the HUD_HEALTH_BUFFER based on the player's health.
    switch (HUDHEALTH) 
    {
    case 25:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 255;
        HUD_HEALTH_BUFFER[2] = 255;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 24:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 223;
        HUD_HEALTH_BUFFER[2] = 225;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 20:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 170;
        HUD_HEALTH_BUFFER[2] = 174;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 15:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 117;
        HUD_HEALTH_BUFFER[2] = 124;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 10:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 64;
        HUD_HEALTH_BUFFER[2] = 73;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 5:
        HUD_HEALTH_BUFFER[0] = 237;
        HUD_HEALTH_BUFFER[1] = 28;
        HUD_HEALTH_BUFFER[2] = 36;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 4:
        HUD_HEALTH_BUFFER[0] = 197;
        HUD_HEALTH_BUFFER[1] = 16;
        HUD_HEALTH_BUFFER[2] = 26;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 3:
        HUD_HEALTH_BUFFER[0] = 148;
        HUD_HEALTH_BUFFER[1] = 12;
        HUD_HEALTH_BUFFER[2] = 18;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 2:
        HUD_HEALTH_BUFFER[0] = 99;
        HUD_HEALTH_BUFFER[1] = 7;
        HUD_HEALTH_BUFFER[2] = 12;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 1:
        HUD_HEALTH_BUFFER[0] = HUD_HEALTH_POS_HEIGHT;
        HUD_HEALTH_BUFFER[1] = 3;
        HUD_HEALTH_BUFFER[2] = 5;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    case 0:
        HUD_HEALTH_BUFFER[0] = 0;
        HUD_HEALTH_BUFFER[1] = 0;
        HUD_HEALTH_BUFFER[2] = 0;
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    default:
        drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);
        break;
    }

    if (stage.score > 0 && stage.score == highscore)
    {
        drawText(HUD_HIGHSCORE_POS_WIDTH, HUD_HIGHSCORE_POS_HEIGHT, 0, 255, 0, "HIGH SCORE: %03d", highscore);
    }
    else
    {
        drawText(HUD_HIGHSCORE_POS_WIDTH, HUD_HIGHSCORE_POS_HEIGHT, 255, 255, 255, "HIGH SCORE: %03d", highscore);
    }
}
/* ---------- */

/* ----- Rendering objects in order ----- */

/* Basic Sorting. */
static void draw(void)
{
    drawBackground();
    drawStars();
    drawFighters(); // Draw the player and enemy entities.
    drawDebris();
    drawExplosions();
    drawtrails();
    drawfire();
    drawBullets(); // Draw the bullets.
    drawHudX();
    drawHud();
    drawHudeffectX();
    drawHudscreen();
}
/* ---------- */