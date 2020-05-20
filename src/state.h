#ifndef STATE_H
#define STATE_H

#include "entity.h"

#define PLAYER_HP  10
#define PLAYER_RAD 10
#define PLAYER_SPEED 4
#define PLAYER_COOLDOWN 8

//Cooldowns for Shooting enemies
#define BLIND_COOLDOWN 30
#define SNIPER_COOLDOWN 50

#define MINION_HP   4
#define MINION_RAD 12

#define BRUTE_HP   12
#define BRUTE_RAD  16


//New Enemy BLIND (Shoots aleatory)
#define BLIND_HP   10
#define BLIND_RAD  20

//New Enemy SNIPER (Shoots following the player)
#define SNIPER_HP   5
#define SNIPER_RAD  10

#define BULLET_DMG 3
#define BULLET_SPEED 16
#define BULLET_RAD   5


// ==== PLAYER DEFINITION
typedef struct {
    entity ent;
    int cooldown;
} player;

// ==== ENEMY DEFINITION
typedef enum {MINION=0, BRUTE=1, BLIND=2, SNIPER=3} enemykind; //add BLIND and SNIPER enemy

typedef struct {
    entity ent;
    int cooldown; //cooldown for shooting enemies
    enemykind kind;
} enemy;

// ==== BULLET DEFINTION
typedef enum {p=0, e=1} typye_bullet; //enum to use in type_bullet

typedef struct {
    entity ent;
    typye_bullet t_bullet; //type_bullet to compare bullets from enemies or from player
} bullet;

// ==== STATE DEFINITION

#define MAX_ENEMIES 128
#define MAX_BULLETS 128

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

} state;



state *state_new();

void state_update(level *lvl, state *sta);

void state_populate_random(level *lvl, state *sta, int n_enemies);

void state_free(state *sta);



#endif
