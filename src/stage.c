/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include "common.h"
#include "background.h"
#include "draw.h"
#include "highscores.h"
#include "sound.h"
#include "stage.h"
#include "text.h"
#include "util.h"
#include "hud.h"

extern App app;
extern Highscores highscore;
extern Stage stage;

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
static void doPointsSphere(void);
static void addPointsSphere(int x, int y);
static void drawPointsSphere(void);
static void drawHudText(void);

static Entity *player;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *EnemyBulletTexture;
static SDL_Texture *playerTexture;
static SDL_Texture *background;
static SDL_Texture *explosionTexture;
static SDL_Texture *trailTexture;
static SDL_Texture *fireTexture;
static SDL_Texture* pointsTexture;
static int enemySpawnTimer;
static int stageResetTimer;
static int POINT_RESULT_BUFFER;
int HUD_HEALTH_BUFFER[3];

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    printf("Initializing the stage...\n");

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;
    stage.trailTail = &stage.trailHead;
    stage.fireTail = &stage.fireHead;
    stage.pointsTail = &stage.pointsHead;

    printf("Loading textures...\n");
    bulletTexture = loadTexture("gfx/playerBullet.png");
    enemyTexture = loadTexture("gfx/enemy.png");
    EnemyBulletTexture = loadTexture("gfx/enemyBullet.png");
    playerTexture = loadTexture("gfx/player.png");
    background = loadTexture("gfx/background.png");
    explosionTexture = loadTexture("gfx/explosion.png");
    trailTexture = loadTexture("gfx/trail.png");
    fireTexture = loadTexture("gfx/fire.png");
    pointsTexture = loadTexture("gfx/points.png");

    //loadMusic("music/voidfighter - Track 01 (deepspace-01).ogg");

    //playMusic(1);

    printf("Stage initialization completed!\n");

    resetStage();
	stage.score = 0;

	initPlayer();

	enemySpawnTimer = 0;

	stageResetTimer = FPS * 3;
}

static void resetStage(void)
{
    Entity *e;
    Explosion *ex;
    Debris *d;
    trail *tr;
    fire *f;

    printf("Resetting the stage...\n");

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

    stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;
	stage.pointsTail = &stage.pointsHead;

    printf("Stage reset completed!\n");
}

static void initPlayer()
{
    printf("Initializing the player...\n");

    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->health = 25;
    player->x = 100;
    player->y = (SCREEN_HEIGHT - HUD_HEIGHT)  /2;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
    player->side = SIDE_PLAYER;

    printf("Player initialized successfully!\n");
}

static void logic(void)
{
    printf("Running logic...\n");

    doBackground();
    doStars();
    doHud();
    doPlayer();
    doEnemies();
    doFighters();
    doPointsSphere();
    doBullets();
    doExplosions();
    doDebris();
    dotrails();
    dofire();
    spawnEnemies();
    clipPlayer();
    clipEnemies();
    checkPlayerEnemyCollisions();

    if (player == NULL && --stageResetTimer <= 0)
    {
        addHighscore(stage.score);

        printf("Stage reset initiated.\n");

        initHighscores();
    }

    printf("Logic completed.\n");
}

static void doPlayer(void)
{
    printf("Handling player actions...\n");

    if (player != NULL)
    {
        player->dx = player->dy = 0;
        player->x = player->x - PLAYER_PUSHBACK;

        if (player->reload > 0)
        {
            player->reload--;
        }

        if (app.keyboard[SDL_SCANCODE_UP])
        {
            player->dy = -PLAYER_SPEED;
            printf("Player moving up.\n");
        }

        if (app.keyboard[SDL_SCANCODE_DOWN])
        {
            player->dy = PLAYER_SPEED;
            printf("Player moving down.\n");
        }

        if (app.keyboard[SDL_SCANCODE_LEFT])
        {
            player->dx = -PLAYER_SPEED;
            printf("Player moving left.\n");
        }

        if (app.keyboard[SDL_SCANCODE_RIGHT])
        {
            player->dx = PLAYER_SPEED + 2.5;
            addtrails((player->x - 1), player->y, 32);
            printf("Player moving right.\n");
        }

        if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0)
        {
            playSound(SND_PLAYER_FIRE, CH_PLAYER);
            fireBullet();
            printf("Player fired a bullet.\n");

            player->reload = PLAYER_FIRE_COOLDOWN;
        }
    }

    printf("Player actions handled.\n");
}

static void fireBullet(void)
{
    printf("Firing player bullet...\n");

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

    printf("Player bullet fired.\n");
}

static void doEnemies(void)
{
    printf("Handling enemy actions...\n");

    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            fireEnemyBullet(e);
            playSound(SND_ENEMY_FIRE, CH_ENEMY_FIRE);
            printf("Enemy fired a bullet.\n");
        }
    }

    printf("Enemy actions handled.\n");
}

static void fireEnemyBullet(Entity *e)
{
	Entity *bullet;

    printf("Enemy bullet created.\n");
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
    printf("Bullet path to player calculated..\n");
	bullet->dx *= ENEMY_BULLET_SPEED;
	bullet->dy *= ENEMY_BULLET_SPEED;

	e->reload = (rand() % FPS * 2);

    printf("Enemy bullet fired.\n");
}

static void doFighters(void)
{
    printf("Updating fighter entities...\n");

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

        if (e != player)
        {
            e->dy = sin(SDL_GetTicks() / FPS / 2) * ENEMY_SPEED;
        }
        prev = e;
    }

    printf("Fighter entities updated.\n");
}

static void doBullets(void)
{
    printf("Updating bullet entities...\n");

    Entity *b, *prev;

    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        b->x += b->dx;
        b->y += b->dy;

        if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > (SCREEN_HEIGHT - HUD_HEIGHT))
        {
            if (b == stage.bulletTail)
            {
                stage.bulletTail = prev;
            }

            prev->next = b->next;
            free(b);
            b = prev;
            printf("Bullet hit an enemy or went out of bounds. Bullet removed.\n");
        }

        prev = b;
    }

    printf("Bullet entities updated.\n");
}

static void addExplosions(int x, int y, int num)
{
    printf("Adding explosions...\n");

    Explosion *e;
    int i;

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

    printf("Explosions added.\n");
}

static void addDebris(Entity *e)
{
    printf("Adding debris from bullet collision...\n");

    Debris *d;
    int x, y, w, h;

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

    printf("Debris added.\n");
}

static void addtrails(int x, int y, int num)
{
    printf("Adding trails...\n");

    trail *e;
    int i;

    for (i = 0; i < num; i++)
    {
        if (i >= 1)
            break;

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

        e->a = rand() % FPS * 1.85;
    }

    printf("Trails added.\n");
}

static void addfire(Entity *e)
{
    printf("Adding fire...\n");

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

    printf("Fire added.\n");
}

static int bulletHitFighter(Entity *b)
{
    printf("Checking if a bullet hits a fighter...\n");

    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
        {
            b->health = 0;
            e->health -= 1;

            addExplosions(e->x, e->y, 32);

            if (e == player)
            {
                playSound(SND_PLAYER_DIE, CH_PLAYER);
                printf("Player hit by a bullet.\n");
            }
            else
            {
                stage.score = stage.score + 1;
                addPointsSphere(e->x + e->w / 2, e->y + e->h / 2);
                playSound(SND_ENEMY_DIE, CH_ANY);
                printf("Enemy hit by a bullet. Score increased.\n");
            }

            int i;

            for (i = 0; i <= 2; i++)
            {
                addfire(e);
            }

            for (i = 0; i <= 1; i++)
            {
                addDebris(e);
            }

            printf("Bullet hit a fighter. Explosions, fire, and debris added.\n");

            return 1;
        }
    }

    printf("No fighter hit by the bullet.\n");

    return 0;
}

static void doPointsSphere(void)
{
    printf("Updating point spheres...\n");

    Entity *e, *prev;

    prev = &stage.pointsHead;

    for (e = stage.pointsHead.next; e != NULL; e = e->next)
    {
        if (e->x < 32)
        {
            e->x = 32;
            e->dx = -e->dx;
        }

        if (e->x + e->w > 928)
        {
            e->x = 928 - e->w;
            e->dx = -e->dx;
        }

        if (e->y < 32)
        {
            e->y = 32;
            e->dy = -e->dy;
        }

        if (e->y + e->h > 672)
        {
            e->y = 672 - e->h;
            e->dy = -e->dy;
        }

        e->x += e->dx;
        e->y += e->dy;

        if (player != NULL && collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h))
        {
            e->health = 0;

            stage.score += POINT_RESULT_BUFFER;

            playSound(SND_POINTS, CH_POINTS);

            printf("Player collected a point sphere. Score increased.\n");
        }

        if (--e->health <= 0)
        {
            if (e == stage.pointsTail)
            {
                stage.pointsTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
            printf("Point sphere removed due to health reaching 0.\n");
        }

        prev = e;
    }

    printf("Point spheres updated.\n");
}

static void addPointsSphere(int x, int y)
{
    printf("Adding a point sphere...\n");

    Entity *e;

    e = malloc(sizeof(Entity));
    memset(e, 0, sizeof(Entity));
    stage.pointsTail->next = e;
    stage.pointsTail = e;

    e->x = x;
    e->y = y;
    e->dx = -(rand() % 5);
    e->dy = (rand() % 5);
    e->health = FPS * 10;
    e->texture = pointsTexture;
    e->side = SIDE_ENEMY;

    SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);

    e->x -= e->w / 2;
    e->y -= e->h / 2;

    printf("Point sphere added.\n");
}

static void spawnEnemies(void)
{
    printf("Spawning enemy entities...\n");

    Entity *enemy;

    if (--enemySpawnTimer <= 0)
    {
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->x = HUDSCREEN_WIDTH;
        enemy->y = rand() % HUDSCREEN_HEIGHT;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->dx = -(2 + (rand() % 4));
        enemy->dy = -100 + (rand() % 200);
		enemy->dy /= 100;

        enemy->side = SIDE_ENEMY;
        enemy->health = 1;

        enemy->reload = FPS * (1 + (rand() % 3));

        enemySpawnTimer = ENEMY_SPAWN_TIME + (rand() % FPS);

        printf("Enemy spawned");
    }
}

static void clipPlayer(void)
{
    printf("Clipping the player's position within the screen...\n");

    if (player != NULL)
    {
        if (player->x < HUDSCREEN_X)
        {
            player->x = HUDSCREEN_X;
            printf("Player's x-coordinate clipped to HUDSCREEN_X.\n");
        }

        if (player->y < HUDSCREEN_Y)
        {
            player->y = HUDSCREEN_Y;
            printf("Player's y-coordinate clipped to HUDSCREEN_Y.\n");
        }

        if (player->x > SCREEN_WIDTH / 2 - SCREEN_BOUNDS * 4)
        {
            player->x = SCREEN_WIDTH / 2 - SCREEN_BOUNDS * 4;
            printf("Player's x-coordinate clipped to the left side of the screen bounds.\n");
        }

        if (player->y > HUDSCREEN_HEIGHT)
        {
            player->y = HUDSCREEN_HEIGHT;
            printf("Player's y-coordinate clipped to HUDSCREEN_HEIGHT.\n");
        }
    }

    printf("Player position clipped.\n");
}

static void clipEnemies(void)
{
    printf("Clipping enemy positions within the screen...\n");

    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e != player)
        {
            if (e->x < HUDSCREEN_X)
            {
                e->health = 0;

                printf("Enemy removed from the game: out of left bounds.\n");
            }

            if (e->y < HUDSCREEN_Y)
            {
                e->y = HUDSCREEN_Y;
                printf("Enemy clipped to HUDSCREEN_Y: out of top bounds.\n");
            }

            if (e->x > HUDSCREEN_WIDTH)
            {
                e->x = HUDSCREEN_WIDTH;
                printf("Enemy clipped to HUDSCREEN_WIDTH: out of right bounds.\n");
            }

            if (e->y > HUDSCREEN_HEIGHT)
            {
                e->y = HUDSCREEN_HEIGHT;
                printf("Enemy clipped to HUDSCREEN_HEIGHT: out of bottom bounds.\n");
            }
        }
    }

    printf("All enemy positions clipped within the screen.\n");
}

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
            for (i = 0; i <= 76; i++) // 76.25 frames (human reaction time on average).
            { 
                playSound(SND_PLAYER_DIE, CH_ANY);
                addExplosions(e->x, e->y, 4);
            }
        }
    }

    printf("Fighter collision check completed.\n");
}

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

    printf("Explosions updated.\n");
}

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

    printf("Debris updated.\n");
}

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

    printf("Trails updated.\n");
}

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

    printf("Fire updated.\n");
}

static void drawFighters(void)
{
    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        blit(e->texture, e->x, e->y);
    }

    printf("Fighters rendered.\n");
}

static void drawBullets(void)
{
    Entity *b;

    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        blit(b->texture, b->x, b->y);
    }

    printf("Bullets rendered.\n");
}

static void drawDebris(void)
{
    Debris *d;

    for (d = stage.debrisHead.next; d != NULL; d = d->next)
    {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }

    printf("Debris rendered.\n");
}

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

    printf("Explosions rendered.\n");
}

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

    printf("Trails rendered.\n");
}

static void drawfire(void)
{
    fire *f;

    for (f = stage.fireHead.next; f != NULL; f = f->next)
    {
        blitRect(fireTexture, &f->rect, f->x, f->y);
    }

    printf("Fire rendered.\n");
}

static void drawPointsSphere(void)
{
    Entity *e;

    for (e = stage.pointsHead.next; e != NULL; e = e->next)
    {
        int POINTHEALTH;
        int POINT_RESULT;
        POINTHEALTH = e->health;

        blit(e->texture, e->x, e->y);

        //drawText(e->x, e->y, 255, 255, 255, "Health: %d", POINTHEALTH);

        switch (POINTHEALTH)
        {
        case 600:
            pointsTexture = loadTexture("gfx/points.png");
            e->texture = pointsTexture;
            POINT_RESULT = 10;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 540:
            pointsTexture = loadTexture("gfx/points10.png");
            e->texture = pointsTexture;
            POINT_RESULT = 9;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 480:
            pointsTexture = loadTexture("gfx/points9.png");
            e->texture = pointsTexture;
            POINT_RESULT = 8;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 420:
            pointsTexture = loadTexture("gfx/points8.png");
            e->texture = pointsTexture;
            POINT_RESULT = 7;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 360:
            pointsTexture = loadTexture("gfx/points7.png");
            e->texture = pointsTexture;
            POINT_RESULT = 6;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 300:
            pointsTexture = loadTexture("gfx/points6.png");
            e->texture = pointsTexture;
            POINT_RESULT = 5;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 240:
            pointsTexture = loadTexture("gfx/points5.png");
            e->texture = pointsTexture;
            POINT_RESULT = 4;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 180:
            pointsTexture = loadTexture("gfx/points4.png");
            e->texture = pointsTexture;
            POINT_RESULT = 3;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 120:
            pointsTexture = loadTexture("gfx/points3.png");
            e->texture = pointsTexture;
            POINT_RESULT = 2;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 60:
            pointsTexture = loadTexture("gfx/points2.png");
            e->texture = pointsTexture;
            POINT_RESULT = 1;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        case 30:
            pointsTexture = loadTexture("gfx/points1.png");
            e->texture = pointsTexture;
            POINT_RESULT = 1;
            POINT_RESULT_BUFFER = POINT_RESULT;
            break;
        }
    }
    printf("Point spheres rendered.\n");
}

static void drawHudText(void)
{
    drawText(HUD_SCORE_POS_WIDTH, HUD_SCORE_POS_HEIGHT, 255, 255, 255, "SCORE: %03d", stage.score);

    int HUDHEALTH;

    if (player == NULL)
    {
        HUDHEALTH = 0;
    }
    else
    {
        HUDHEALTH = player->health;
    }

    switch (HUDHEALTH)
    {
    case 25:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 255;
        HUD_HEALTH_BUFFER[2] = 255;
        break;
    case 24:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 223;
        HUD_HEALTH_BUFFER[2] = 225;
        break;
    case 20:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 170;
        HUD_HEALTH_BUFFER[2] = 174;
        break;
    case 15:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 117;
        HUD_HEALTH_BUFFER[2] = 124;
        break;
    case 10:
        HUD_HEALTH_BUFFER[0] = 255;
        HUD_HEALTH_BUFFER[1] = 64;
        HUD_HEALTH_BUFFER[2] = 73;
        break;
    case 5:
        HUD_HEALTH_BUFFER[0] = 237;
        HUD_HEALTH_BUFFER[1] = 28;
        HUD_HEALTH_BUFFER[2] = 36;
        break;
    case 4:
        HUD_HEALTH_BUFFER[0] = 197;
        HUD_HEALTH_BUFFER[1] = 16;
        HUD_HEALTH_BUFFER[2] = 26;
        break;
    case 3:
        HUD_HEALTH_BUFFER[0] = 148;
        HUD_HEALTH_BUFFER[1] = 12;
        HUD_HEALTH_BUFFER[2] = 18;
        break;
    case 2:
        HUD_HEALTH_BUFFER[0] = 99;
        HUD_HEALTH_BUFFER[1] = 7;
        HUD_HEALTH_BUFFER[2] = 12;
        break;
    case 1:
        HUD_HEALTH_BUFFER[0] = HUD_HEALTH_POS_HEIGHT;
        HUD_HEALTH_BUFFER[1] = 3;
        HUD_HEALTH_BUFFER[2] = 5;
        break;
    case 0:
        HUD_HEALTH_BUFFER[0] = 0;
        HUD_HEALTH_BUFFER[1] = 0;
        HUD_HEALTH_BUFFER[2] = 0;
        break;
    default:
        break;
    }

    drawText(HUD_HEALTH_POS_WIDTH, HUD_HEALTH_POS_HEIGHT, HUD_HEALTH_BUFFER[0], HUD_HEALTH_BUFFER[1], HUD_HEALTH_BUFFER[2], "HEALTH: %03d", HUDHEALTH);

    if (stage.score > 0)
    {
        drawText(HUD_HIGHSCORE_POS_WIDTH, HUD_HIGHSCORE_POS_HEIGHT, 0, 255, 0, "HIGH SCORE: %03d", stage.score);
    }
    else
    {
        drawText(HUD_HIGHSCORE_POS_WIDTH, HUD_HIGHSCORE_POS_HEIGHT, 255, 255, 255, "HIGH SCORE: %03d", stage.score);
    }

    printf("HUD text rendered.\n");
}

static void draw(void)
{
    drawBackground();
    drawStars();
    drawPointsSphere();
    drawFighters();
    drawDebris();
    drawExplosions();
    drawtrails();
    drawfire();
    drawBullets();
    drawHud();
    drawHudText();
    drawHudEffects();

    printf("Rendering completed.\n");
}