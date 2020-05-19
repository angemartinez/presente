#include "draw.h"

#include <raylib.h>
#include <math.h>
#define RAYGUI_IMPLEMENTATION

void draw_level(const level *lvl){
    // Draw cells, iterate, including borders outside the level
    for(int y=-1;y<=lvl->size_y;y++){
        for(int x=-1;x<=lvl->size_x;x++){
            char cell = level_get(lvl,x,y);
            // If the cell is a wall, paint it
            if(cell=='#'){
                DrawRectangle(TILE_SIZE*x,TILE_SIZE*y,TILE_SIZE,TILE_SIZE,BROWN);
            }
        }
    }
}

void draw_state(const level *lvl, const state *sta){

    // Initialize a camera to be used as drawing context
    Camera2D cam;
    // make the camera follow the player
    cam.target.x = sta->pla.ent.x;
    cam.target.y = sta->pla.ent.y;
    // make it so that the player is in the middle of the screen
    cam.offset.x = GetScreenWidth()/2.0;
    cam.offset.y = GetScreenHeight()/2.0;
    // set the camera rotation to 0
    cam.rotation = 0.0;
    // set the camera zoom to 1
    cam.zoom = 1.0;

    // Draw everything relative to the camera from now on
    BeginMode2D(cam);

    // Draw level
    draw_level(lvl);

    // Draw enemies
    for(int i=0;i<sta->n_enemies;i++){
        // Get a copy of the enemy entity
        entity ent = sta->enemies[i].ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        // Draw a circle with the radius of the entity, color depends on the enemy type
        if(sta->enemies[i].kind == MINION){
            DrawCircleV(vec,ent.rad,YELLOW);
        }else{
            DrawCircleV(vec,ent.rad,RED);
        }
    }

    // Draw player
    {
        // Get a copy of the player's entity
        entity ent = sta->pla.ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        // Draw a circle with the radius of the entity
        DrawCircleV(vec,ent.rad,BLUE);
    }

    // Draw bullets
    for(int i=0;i<sta->n_bullets;i++){
        // Get a copy of the bullet entity
        entity ent = sta->bullets[i].ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        // Draw a circle with the radius of the entity
        DrawCircleV(vec,ent.rad,PINK);
    }

    //draw helth bar     https://knowyourmeme.com/memes/helth
    
    {
    

        float radioI = 40.0f;
        float radioE = 60.0f;
        Vector2 lugar = {cam.target.x+320,cam.target.y+230};   //320,240

        int AnguloI=0-(36*(sta->pla.ent.hp-10));
        int AnguloF=360;  //arreglar
        int segmentos= sta->pla.ent.hp; 

        DrawRing(lugar,radioI, radioE, AnguloI, AnguloF, segmentos, Fade(GREEN, 0.6f));
        DrawRingLines(lugar, radioI, radioE, AnguloI, AnguloF, segmentos, Fade(BLACK, 0.4));

        
    }

    //draw enemi helth
    {
        
        for(int k=0;k<sta->n_enemies;k++){
                int enemiH = sta->enemies[k].ent.hp;
                float enemiX = sta->enemies[k].ent.x;
                float enemiY = sta->enemies[k].ent.y;
                //draw
                DrawText(FormatText("%d", enemiH),enemiX,enemiY+15,12,BLACK);
                }
    }
    //draw cooldown
    {
        float ri= 13.0f;
        float re= 15.0f;
        Vector2 recarga = {cam.target.x,cam.target.y};    //no se me ocurrio otra forma de decir cooldown sin dejar la escoba

        int Ai=0;
        int Af=0;
        if(sta->pla.cooldown >= 0) {  //aparece un circulo cuando no se pueda disparar 
            Af=360;
            }
        else {
            Af=0;
        }
        int seg=0;

        DrawRing(recarga,ri,re,Ai,Af,seg,Fade(DARKGRAY,0.8f));
    } 
    //draw eim
    {
    float mouse_y = GetMouseY()-GetScreenHeight()/2;
    float mouse_x = GetMouseX()-GetScreenWidth()/2;

    Vector2 mira= {cam.target.x + mouse_x ,cam.target.y + mouse_y };

    DrawCircleV(mira,5,DARKGRAY);
    }
        //linea apuntando al mouse
    // Stop drawing relative to the camera
    EndMode2D();
}


//agregar
// void DisableCursor(void);