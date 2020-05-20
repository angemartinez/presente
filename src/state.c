#include "state.h"

#include <raylib.h>
#include <time.h> //time.h to use srand
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

state *state_new(){

    // Intialize srand for random numbers generation
    srand(time(NULL));

    state *sta = malloc(sizeof(state));

    memset(sta,0,sizeof(state));

    sta->pla.ent.x = TILE_SIZE/2;
    sta->pla.ent.y = TILE_SIZE/2;
    sta->pla.ent.rad = PLAYER_RAD;
    sta->pla.ent.hp  = PLAYER_HP;

    return sta;
}

void state_update(level *lvl, state *sta){

    //print in terminal the HP of the player
    //printf("%d\n",sta->pla.ent.hp);
    
    // == Update player speed accordin to buttons
    
    float mov_x = 0;
    float mov_y = 0;
    mov_x += sta->button_state[0];
    mov_x -= sta->button_state[2];
    mov_y -= sta->button_state[1];
    mov_y += sta->button_state[3];
    float mov_norm = sqrt(mov_x*mov_x+mov_y*mov_y);

    if(mov_norm==0){
        sta->pla.ent.vx *= 0.6;
        sta->pla.ent.vy *= 0.6;
    }else{
        sta->pla.ent.vx = mov_x/mov_norm * PLAYER_SPEED;
        sta->pla.ent.vy = mov_y/mov_norm * PLAYER_SPEED;
    }

    // Create bullets for player
    sta->pla.cooldown -= 1;
    if(sta->button_state[4] && sta->pla.cooldown<=0){
        sta->pla.cooldown = PLAYER_COOLDOWN;
        if(sta->n_bullets<MAX_BULLETS){
            sta->bullets[sta->n_bullets].t_bullet = p; //t_bullet for type_bullet
            sta->bullets[sta->n_bullets].ent        = sta->pla.ent;
            sta->bullets[sta->n_bullets].ent.vx     =  BULLET_SPEED*cos(sta->aim_angle);
            sta->bullets[sta->n_bullets].ent.vy     = -BULLET_SPEED*sin(sta->aim_angle);
            sta->bullets[sta->n_bullets].ent.rad    = BULLET_RAD;
            sta->bullets[sta->n_bullets].ent.hp     = BULLET_DMG;
            sta->n_bullets += 1;
        }
    }
    //Create Bullets For blind Enemy
    for(int k=0;k<sta->n_enemies;k++){
        if(sta->enemies[k].kind == 2){
            int r_numb = rand(); //generate a random number
            sta->enemies[k].cooldown -= 1;
            if(sta->enemies[k].cooldown<=0){
                sta->enemies[k].cooldown = BLIND_COOLDOWN;
                if(sta->n_bullets<MAX_BULLETS){
                    if(sta->enemies[k].ent.rad){
                        sta->bullets[sta->n_bullets].t_bullet = e; //t_bullet for type_bullet
                        sta->bullets[sta->n_bullets].ent        = sta->enemies[k].ent;
                        sta->bullets[sta->n_bullets].ent.vx     =  BULLET_SPEED*cos(r_numb);
                        sta->bullets[sta->n_bullets].ent.vy     = -BULLET_SPEED*sin(r_numb);
                        sta->bullets[sta->n_bullets].ent.rad    = BULLET_RAD;
                        sta->bullets[sta->n_bullets].ent.hp     = BULLET_DMG;
                        sta->n_bullets += 1;
                    }
                }
            }
        }
    }
    
    //Create Bullets For Sniper Enemy
    for(int k=0;k<sta->n_enemies;k++){
        if(sta->enemies[k].kind == 3){
            float x_sniper = sta->pla.ent.x - sta->enemies[k].ent.x; //get x component of player-enemy
            float y_sniper = sta->pla.ent.y - sta->enemies[k].ent.y; //get y component of player-enemy
            sta->enemies[k].cooldown -= 1;
            if(sta->enemies[k].cooldown<=0){
                sta->enemies[k].cooldown = SNIPER_COOLDOWN;
                if(sta->n_bullets<MAX_BULLETS){
                    if(sta->enemies[k].ent.rad){
                        sta->bullets[sta->n_bullets].t_bullet = e; //t_bullet for type_bullet 
                        sta->bullets[sta->n_bullets].ent        = sta->enemies[k].ent;
                        sta->bullets[sta->n_bullets].ent.vx     =  BULLET_SPEED*cos(atan2(-y_sniper,x_sniper)); //uses atan2 to shoot following the player
                        sta->bullets[sta->n_bullets].ent.vy     = -BULLET_SPEED*sin(atan2(-y_sniper,x_sniper)); //uses atan2 to shoot following the player
                        sta->bullets[sta->n_bullets].ent.rad    = BULLET_RAD;
                        sta->bullets[sta->n_bullets].ent.hp     = BULLET_DMG;
                        sta->n_bullets += 1;
                    }
                }
            }
        }
    }

    // Check bullet-enemy collisions
    for(int i=0;i<sta->n_bullets;i++){
        for(int k=0;k<sta->n_enemies;k++){
            if(sta->bullets[i].t_bullet == 0){ //uses t_bullet to compare bullets from player or enemy
                if(entity_collision(&sta->bullets[i].ent,&sta->enemies[k].ent)){
                    sta->enemies[k].ent.hp -= sta->bullets[i].ent.hp;
                    sta->bullets[i].ent.dead = 1;
                } 
            }
        }
    }
    //Check Bullet-Player Collisions
    for(int i=0;i<sta->n_bullets;i++){
        if(sta->bullets[i].t_bullet == 1){ //uses t_bullet to compare bullets from player or enemy
            if(entity_collision(&sta->bullets[i].ent,&sta->pla.ent)){
                sta->pla.ent.hp -= sta->bullets[i].ent.hp;
                sta->bullets[i].ent.dead = 1;
                
            } 
        }
    }
    

    // Update physics
    entity_physics(lvl,&sta->pla.ent);
    for(int i=0;i<sta->n_enemies;i++){
        entity_physics(lvl,&sta->enemies[i].ent);
        if(sta->enemies[i].ent.hp<=0) sta->enemies[i].ent.dead = 1;
    }
    for(int i=0;i<sta->n_bullets;i++){
        int col = entity_physics(lvl,&sta->bullets[i].ent);
        if(col) sta->bullets[i].ent.dead = 1;
    }



    // Delete dead entities:
    {
        int new_n_bullets = 0;
        for(int i=0;i<sta->n_bullets;i++){
            if(!sta->bullets[i].ent.dead){
                sta->bullets[new_n_bullets] = sta->bullets[i];
                new_n_bullets += 1;
            }
        }
        sta->n_bullets = new_n_bullets;
    }

    {
        int new_n_enemies = 0;
        for(int i=0;i<sta->n_enemies;i++){
            if(!sta->enemies[i].ent.dead){
                sta->enemies[new_n_enemies] = sta->enemies[i];
                new_n_enemies += 1;
            }
        }
        sta->n_enemies = new_n_enemies;
    }
    

}

void state_populate_random(level *lvl, state *sta, int n_enemies){
    assert(n_enemies<=MAX_ENEMIES);
    while(sta->n_enemies<n_enemies){
        while(1){
            int posx = rand()%lvl->size_x;
            int posy = rand()%lvl->size_y;
            if(level_get(lvl,posx,posy)!='#'){
                memset(&sta->enemies[sta->n_enemies],0,sizeof(enemy));

                sta->enemies[sta->n_enemies].ent.x = (posx+0.5)*TILE_SIZE;
                sta->enemies[sta->n_enemies].ent.y = (posy+0.5)*TILE_SIZE;
                int brute = rand()%2==0;
                if(brute){
                    int blind = rand()%2==0;
                    if(blind){ //generate a BLIND enemy
                        sta->enemies[sta->n_enemies].kind   = BLIND;
                        sta->enemies[sta->n_enemies].ent.hp = BLIND_HP;
                        sta->enemies[sta->n_enemies].ent.rad = BLIND_RAD;
                    }else{
                        sta->enemies[sta->n_enemies].kind   = BRUTE;
                        sta->enemies[sta->n_enemies].ent.hp = BRUTE_HP;
                        sta->enemies[sta->n_enemies].ent.rad = BRUTE_RAD;
                    } 
                }else{
                    int sniper = rand()%2==0;
                    if(sniper){ //generate a SNIPER enemy
                        sta->enemies[sta->n_enemies].kind   = SNIPER;
                        sta->enemies[sta->n_enemies].ent.hp = SNIPER_HP;
                        sta->enemies[sta->n_enemies].ent.rad = SNIPER_RAD;
                    }else{
                        sta->enemies[sta->n_enemies].kind   = MINION;
                        sta->enemies[sta->n_enemies].ent.hp = MINION_HP;
                        sta->enemies[sta->n_enemies].ent.rad = MINION_RAD;
                    }
                }
                sta->n_enemies++;
                break;
            }
        }
    }
}

void state_free(state *sta){
    free(sta);
}
