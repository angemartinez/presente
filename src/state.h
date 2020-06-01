#ifndef STATE_H
#define STATE_H
#include "entity.h"

#define ENEMY_FOV 240
#define MAX_SPD
#define PLAYER_HP 10
#define PLAYER_RAD 10
#define PLAYER_SPEED 4
#define PLAYER_COOLDOWN 8


#define MINION_HP   4
#define MINION_RAD 12

#define BRUTE_HP   12
#define BRUTE_RAD  16

#define SENTRY_HP 20
#define SENTRY_RAD 20
#define SENTRY_COOLDOWN 20

#define BLIND_HP   10
#define BLIND_RAD  20
#define BLIND_COOLDOWN 30

#define SNIPER_HP   5
#define SNIPER_RAD  10
#define SNIPER_COOLDOWN 50

#define BULLET_DMG 3
#define BULLET_SPEED 9
#define BULLET_RAD 5


// ==== PLAYER DEFINITION
typedef struct{
    entity ent;
    int cooldown;
} player;

// ==== ENEMY DEFINITION
typedef enum {
    MINION=0,
    BRUTE=1,
    SENTRY=2,
    BLIND=3,
    SNIPER=4
} enemykind; //add BLIND and SNIPER enemy

typedef struct {
    entity ent;
    enemykind kind;

    int cooldown; //cooldown for shooting enemies
} enemy;

// ==== BULLET DEFINTION
typedef struct{
    //Modified 8, added field "tipo". 0 if it's shooted by player, otherwise is 1, Issue #3
    int tipo;
    entity ent;
} bullet;

// ==== STATE DEFINITION

#define MAX_ENEMIES 128
#define MAX_BULLETS 200

#define N_BUTTONS 5

typedef struct {
    player pla;

    int n_enemies;
    enemy enemies[MAX_ENEMIES];

    int n_bullets;
    bullet bullets[MAX_BULLETS];

    // Controls
    int button_state[N_BUTTONS];
    float aim_angle;
    // Added number of frame (ISSUE 14)
    int frame;

} state;



state *state_new();

void state_update(level *lvl, state *sta);

void state_populate_random(level *lvl, state *sta, int n_enemies);

void state_free(state *sta);

#endif
