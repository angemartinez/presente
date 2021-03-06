#include "state.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <raylib.h>
state *state_new()
{
    // Ask for memory for the state
    state *sta = malloc(sizeof(state));

    // Fill every byte in the state with 0's so that effectivelly each field is set to 0.
    // (this is a trick from <string.h>)
    memset(sta, 0, sizeof(state));

    // We put the player in the center of the top-left cell.
    sta->pla.ent.x = TILE_SIZE / 2;
    sta->pla.ent.y = TILE_SIZE / 2;
    sta->pla.ent.rad = PLAYER_RAD;
    sta->pla.ent.hp = PLAYER_HP;

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

    float mov_norm = sqrt(mov_x * mov_x + mov_y * mov_y);
    if (sta->n_enemies == 0){ //Modified 1, player wins the game
        sta->pla.ent.vx = 0;
        sta->pla.ent.vy = 0;
        sta->pla.ent.dead = 1;
    }
    if (mov_norm == 0 || sta->pla.ent.dead){
        // If nothing is being pressed, deacelerate the player
        sta->pla.ent.vx *= 0.6;
        sta->pla.ent.vy *= 0.6;
    }else{
        // If something is being pressed, normalize the mov vector and multiply by the PLAYER_SPEED
        sta->pla.ent.vx = mov_x / mov_norm * PLAYER_SPEED;
        sta->pla.ent.vy = mov_y / mov_norm * PLAYER_SPEED;
    }

    // Create bullets for player
    sta->pla.cooldown -= 1;
    // If the shoot button is pressed and the player cooldown is smaller than 0, shoot a bullet
    if (sta->button_state[4] && sta->pla.cooldown <= 0 && !sta->pla.ent.dead){
        // Reset the player cooldown to a positive value so that he can't shoot for that amount of frames
        sta->pla.cooldown = PLAYER_COOLDOWN;
        // Ensure that the new bullet won't be created if that would overflow the bullets array
        if (sta->n_bullets < MAX_BULLETS){
            // The new bullet will be in the next unused position of the bullets array
            bullet *new_bullet = &sta->bullets[sta->n_bullets];
            sta->n_bullets += 1;
            // Initialize all bullet fields to 0
            memset(new_bullet, 0, sizeof(bullet));
            // Start the bullet on the player's position
            new_bullet->ent.x = sta->pla.ent.x;
            new_bullet->ent.y = sta->pla.ent.y;
            // Bullet speed is set to the aiming angle
            new_bullet->ent.vx = BULLET_SPEED * cos(sta->aim_angle);
            new_bullet->ent.vy = -BULLET_SPEED * sin(sta->aim_angle);
            //
            new_bullet->ent.rad = BULLET_RAD;
            new_bullet->ent.hp = BULLET_DMG;
            new_bullet->tipo = 0;
        }
    }

    for(int k=0;k<sta->n_enemies;k++){
        int r_numb = rand(); //generate a random number
        sta->enemies[k].cooldown -= 1;
        if(sta->enemies[k].cooldown<=0){
            if(sta->n_bullets<MAX_BULLETS){
                if (sta->enemies[k].kind == SENTRY){
                    bullet *new_bullet = &sta->bullets[sta->n_bullets];
                    sta->n_bullets += 1;
                    memset(new_bullet, 0, sizeof(bullet));

                    float delta_x = sta->pla.ent.x - sta->enemies[k].ent.x;
                    float delta_y = sta->pla.ent.y - sta->enemies[k].ent.y;
                    float delta_mag = sqrt(delta_x*delta_x+delta_y*delta_y);

                    // Midpoint of the midpoint between player and sentry enemy, x axis
                    new_bullet->ent.x = (((sta->enemies[k].ent.x + sta->pla.ent.x) / 2) + sta->enemies[k].ent.x) / 2;
                    // Midpoint of the midpoint between player and sentry enemy, y axis
                    new_bullet->ent.y = (((sta->enemies[k].ent.y + sta->pla.ent.y) / 2) + sta->enemies[k].ent.y) / 2;
                    // Bullet direction will be the first position where sentry detected the player
                    if (new_bullet->ent.vx == 0 && new_bullet->ent.vy == 0){
                        new_bullet->ent.vx = delta_x / delta_mag * 3.5;
                        new_bullet->ent.vy = delta_y / delta_mag * 3.5;
                    }
                    new_bullet->tipo = 1;
                    //
                    new_bullet->ent.rad = BULLET_RAD;
                    new_bullet->ent.hp = BULLET_DMG;

                    sta->enemies[k].cooldown = SENTRY_COOLDOWN;

                } else if(sta->enemies[k].kind == BLIND){
                    bullet *new_bullet = &sta->bullets[sta->n_bullets];
                    sta->n_bullets += 1;
                    memset(new_bullet, 0, sizeof(bullet));

                    new_bullet->ent        = sta->enemies[k].ent;
                    new_bullet->ent.vx     =  BULLET_SPEED*cos(r_numb);
                    new_bullet->ent.vy     = -BULLET_SPEED*sin(r_numb);
                    new_bullet->ent.rad    = BULLET_RAD;
                    new_bullet->ent.hp     = BULLET_DMG;
                    new_bullet->tipo = 1;

                    sta->enemies[k].cooldown = BLIND_COOLDOWN;
                }
                //Create Bullets For Sniper Enemy
                else if(sta->enemies[k].kind == SNIPER){
                    float x_sniper = sta->pla.ent.x - sta->enemies[k].ent.x; //get x component of player-enemy
                    float y_sniper = sta->pla.ent.y - sta->enemies[k].ent.y; //get y component of player-enemy

                    bullet *new_bullet = &sta->bullets[sta->n_bullets];
                    sta->n_bullets += 1;
                    memset(new_bullet, 0, sizeof(bullet));

                    new_bullet->ent        = sta->enemies[k].ent;
                    new_bullet->ent.vx     =  BULLET_SPEED*cos(atan2(-y_sniper,x_sniper)); //uses atan2 to shoot following the player
                    new_bullet->ent.vy     = -BULLET_SPEED*sin(atan2(-y_sniper,x_sniper)); //uses atan2 to shoot following the player
                    new_bullet->ent.rad    = BULLET_RAD;
                    new_bullet->ent.hp     = BULLET_DMG;
                    new_bullet->tipo = 1;

                    sta->enemies[k].cooldown = SNIPER_COOLDOWN;
                }
            }
        }
    }

    // #Modified 3, Enemy movement,  Issue #7
    for (int i = 0; i < sta->n_enemies; i++){
        float delta_x = sta->pla.ent.x - sta->enemies[i].ent.x;
        float delta_y = sta->pla.ent.y - sta->enemies[i].ent.y;
        float delta_mag = sqrt(delta_x * delta_x + delta_y * delta_y);
        if (delta_mag <= ENEMY_FOV || delta_mag == 0){ //Until player is not in enemy_fov or distance between them is 0
            if (sta->enemies[i].kind == MINION){
                sta->enemies[i].ent.vx = (delta_x / delta_mag) * PLAYER_SPEED;
                sta->enemies[i].ent.vy = (delta_y / delta_mag) * PLAYER_SPEED;
            }
            else if (sta->enemies[i].kind == BRUTE){ //BRUTE enemies will move to the first position where they detected the player with constant speed
                if (sta->enemies[i].ent.vx == 0 && sta->enemies[i].ent.vy == 0){
                    sta->enemies[i].ent.vx = (delta_x / delta_mag) * 3;
                    sta->enemies[i].ent.vy = (delta_y / delta_mag) * 3;

                    // Because BRUTE enemies will move to the FIRST position, i saved the first distance between them
                    sta->enemies[i].ent.save_distance = delta_mag;
                }
                // case where BRUTE enemy has speed and player is in its FOV
                else{
                    if (sta->enemies[i].ent.save_distance > 0){
                        float cateto1 = sta->enemies[i].ent.vx * sta->enemies[i].ent.vx;
                        float cateto2 = sta->enemies[i].ent.vy * sta->enemies[i].ent.vy;
                        float euclidean_dist = sqrt(cateto1 + cateto2);
                        sta->enemies[i].ent.save_distance -= euclidean_dist;
                    }
                    else{ // enemy reach player pos
                        sta->enemies[i].ent.vx = 0;
                        sta->enemies[i].ent.vy = 0;
                    }
                }
            }
        } else { // If enemy is kind BRUTE and has speed while not in player FOV, reduce its speed
            if (sta->enemies[i].kind == BRUTE){
                if (sta->enemies[i].ent.save_distance > 0){
                    //  reduce save_distance by euclidean distance of its speed vector
                    sta->enemies[i].ent.save_distance -= sqrt((sta->enemies[i].ent.vx * sta->enemies[i].ent.vx) + (sta->enemies[i].ent.vy * sta->enemies[i].ent.vy));
                } else { //enemy reach player pos
                    sta->enemies[i].ent.vx = 0;
                    sta->enemies[i].ent.vy = 0;
                }
            }
        }
    }

    // == Check bullet-player collisions
    for (int i = 0; i < sta->n_bullets; i++){
        if (entity_collision(&sta->bullets[i].ent, &sta->pla.ent)){
            if (sta->bullets[i].tipo == 1){ // Enemy bullet, only damages player.
                sta->pla.ent.hp -= sta->bullets[i].ent.hp;
                sta->bullets[i].ent.dead = 1;
            }
        }
    }

    // Check bullet-enemy collisions
    for(int i=0;i<sta->n_bullets;i++){
        for(int k=0;k<sta->n_enemies;k++){
            // If a bullet is colliding with an enemy
            if (entity_collision(&sta->bullets[i].ent, &sta->enemies[k].ent)){
                // Reduce enemy's health by bullet's health and kills bullet
                if (sta->bullets[i].tipo == 0){ // Player bullet. Only damages enemies.
                    sta->enemies[k].ent.hp -= sta->bullets[i].ent.hp;
                    sta->bullets[i].ent.dead = 1;
                }
            }
        }
    }


    // == Update entities
    // Update player
    // #Modified 5, if player's hp is <= 0, prints "GAME OVER!"
    entity_physics(lvl, &sta->pla.ent);
    if (sta->pla.ent.hp <= 0){
        sta->pla.ent.dead = 1;
    }
    // Update enemies
    for (int i = 0; i < sta->n_enemies; i++){
        entity_physics(lvl, &sta->enemies[i].ent);
        // Kill enemy if it has less than 0 HP
        if (sta->enemies[i].ent.hp <= 0){
            sta->enemies[i].ent.dead = 1;
            printf("Enemy kind %d died.\n",sta->enemies[i].kind);
        }
    }
    // Update bullets
    for (int i = 0; i < sta->n_bullets; i++){
        int col = entity_physics(lvl, &sta->bullets[i].ent);
        // Kill bullet if it is colliding with a wall
        if (col) sta->bullets[i].ent.dead = 1;
    }

    // == Delete dead entities
    {
        int new_n_bullets = 0;
        for (int i = 0; i < sta->n_bullets; i++)
        {
            if (!sta->bullets[i].ent.dead)
            {
                sta->bullets[new_n_bullets] = sta->bullets[i];
                new_n_bullets += 1;
            }
        }
        sta->n_bullets = new_n_bullets;
    }

    {
        int new_n_enemies = 0;
        for (int i = 0; i < sta->n_enemies; i++)
        {
            if (!sta->enemies[i].ent.dead)
            {
                sta->enemies[new_n_enemies] = sta->enemies[i];
                new_n_enemies += 1;
            }
        }
        sta->n_enemies = new_n_enemies;
    }
}

void state_populate_random(level *lvl, state *sta, int n_enemies){
    assert(n_enemies <= MAX_ENEMIES);
    while (sta->n_enemies < n_enemies){
        // Until an empty cell is found, Las Vegas algorithm approach.
        while (1){
            int posx = rand() % lvl->size_x;
            int posy = rand() % lvl->size_y;
            // Check if the cell is empty
            if (level_get(lvl, posx, posy) == '.'){

                // The new enemy will be in the next unused position of the enemies array
                enemy *new_enemy = &sta->enemies[sta->n_enemies];
                sta->n_enemies++;

                // Initialize all new enemy fields to 0
                memset(new_enemy, 0, sizeof(enemy));

                // Put the new enemy at the center of the chosen cell
                new_enemy->ent.x = (posx + 0.5) * TILE_SIZE;
                new_enemy->ent.y = (posy + 0.5) * TILE_SIZE;

                // Pick an enemy type and set variables accordingly
                int typedice = rand()%10;

                switch(typedice){
                    case 0:
                        new_enemy->kind = BRUTE;
                        new_enemy->ent.hp = BRUTE_HP;
                        new_enemy->ent.rad = BRUTE_RAD;
                    break;
                    case 1:
                        new_enemy->kind = SENTRY;
                        new_enemy->ent.hp = SENTRY_HP;
                        new_enemy->ent.rad = SENTRY_RAD;
                        new_enemy->cooldown = SENTRY_COOLDOWN;
                    break;
                    case 2:
                        new_enemy->kind    = SNIPER;
                        new_enemy->ent.hp  = SNIPER_HP;
                        new_enemy->ent.rad = SNIPER_RAD;
                    break;
                    case 3:
                        new_enemy->kind    = BLIND;
                        new_enemy->ent.hp  = BLIND_HP;
                        new_enemy->ent.rad = BLIND_RAD;
                    break;
                    default: // More chance
                        new_enemy->kind = MINION;
                        new_enemy->ent.hp = MINION_HP;
                        new_enemy->ent.rad = MINION_RAD;
                    break;
                }

                break;
            }
        }
    }
}


void state_free(state *sta){
    free(sta);
}
