#include "draw.h"

#include <raylib.h>
#include <stdio.h>

// Now doesnt draw a brown rect
void draw_level(const level *lvl){
    // ISSUE 14 drawing walls and floor 

    // The textures, has many textures inside, so you have to define a part.
    // Example the lvl texture has inside textures for diferent parts of the wall and floor
    Rectangle frame_floor = { 57.0f, 57.0f, 48, 48};
    Rectangle frame_hor_wall={ 52.0f, 0.0f, 48, 48};
    Rectangle frame_vert_wall={ 180.0f, 257.0f, 48, 48};
    Rectangle frame_out_wall={ 55.0f, 200.0f, 48, 48};

    // Draw cells, iterate, including borders outside the level
    for(int y=-1;y<=lvl->size_y;y++){
        for(int x=-1;x<=lvl->size_x;x++){
            // get all nearby cells, center, right, left, up and down.
            char cell = level_get(lvl,x,y);
            char cellr = level_get(lvl,x+1,y);
            char celll = level_get(lvl,x-1,y);
            char cellu = level_get(lvl,x,y-1);
            char celld = level_get(lvl,x,y+1);
            // If the cell is a wall, paint it
            if((cell=='#')&&(cellr=='#')&&(celll=='#')&&(cellu!='#')&&(celld!='#')){
                Vector2 position = { TILE_SIZE*x,TILE_SIZE* y};
                DrawTextureRec(lvl->texture_lvl,frame_hor_wall,position,RAYWHITE);
            }else if((cell=='#')&&(cellu=='#')&&(celld=='#')){
                Vector2 position = { TILE_SIZE*x,TILE_SIZE* y};
                DrawTextureRec(lvl->texture_lvl,frame_vert_wall,position,RAYWHITE); 
            }else if((cell=='#')&&(celld!='#')){
                Vector2 position = { TILE_SIZE*x,TILE_SIZE* y};
                DrawTextureRec(lvl->texture_lvl,frame_hor_wall,position,RAYWHITE);                
            }else if((cell=='#')&&(cellu!='#')&&(celld=='#')){
                Vector2 position = { TILE_SIZE*x,TILE_SIZE* y};
                DrawTextureRec(lvl->texture_lvl,frame_vert_wall,position,RAYWHITE);
            }else if((cell=='#')&&(cellu=='#')&&(celld=='#')&&(celll=='#')&&(cellr=='#')){
                Vector2 position = { TILE_SIZE*x,TILE_SIZE* y};
                DrawTextureRec(lvl->texture_lvl,frame_vert_wall,position,RAYWHITE);                                                         
            }else{
                Vector2 position = { TILE_SIZE*x,TILE_SIZE* y};
                DrawTextureRec(lvl->texture_lvl,frame_floor,position,RAYWHITE);
            }
        }
    }
    // i choose a different texture for the wall in size_y row, if the dungeon is not a square , remove this part and the Rectangle frame_out_wall
    for(int x=-1;x<=lvl->size_x;x++){
            Vector2 position = { TILE_SIZE*x,lvl->size_y*TILE_SIZE};
            DrawTextureRec(lvl->texture_lvl,frame_out_wall,position,RAYWHITE);
    }
    // ------------------------------------------------------------------------------------------------------------------

}

// ISSUE 14 draw dynamic objects

void draw_dynamic_objects(const level *lvl, const state *sta){
    // Torchs for hor wall
    for(int y=-1;y<=lvl->size_y;y++){
        for(int x=-1;x<=lvl->size_x;x+=3){
            char cell = level_get(lvl,x,y);
            char celld = level_get(lvl,x,y+1);
            if((cell=='#')&&(celld!='#')){
                Vector2 position = {TILE_SIZE*x+(TILE_SIZE-16)/2,TILE_SIZE* y+TILE_SIZE/4};
                Rectangle frame_torch={ sta->frame*16.0f, 0.0f, 16, 26};
                DrawTextureRec(lvl->texture_objects,frame_torch,position,RAYWHITE);   
            }
        }
    } 
    // Flags
    for(int y=0;y<=lvl->size_y;y++){
        for(int x=-1;x<=lvl->size_x;x+=6){
            char cell = level_get(lvl,x,y);
            char celld = level_get(lvl,x,y+1);
            if((cell=='#')&&(celld!='#')){
                Vector2 position = {TILE_SIZE*x+(TILE_SIZE-28)/2,TILE_SIZE* y+TILE_SIZE/4};
                Rectangle frame_flag={ sta->frame*28.0f, 86.0f, 28, 26};
                DrawTextureRec(lvl->texture_objects,frame_flag,position,RAYWHITE);   
            }
        }
    } 
    // Torchs for vertical walls 
    for(int y=-1;y<=lvl->size_y;y+=2){
        for(int x=-1;x<=lvl->size_x;x++){
            char cell = level_get(lvl,x,y);
            char cellr = level_get(lvl,x+1,y);
            char celll = level_get(lvl,x-1,y);
            char cellu = level_get(lvl,x,y-1);
            char celld = level_get(lvl,x,y+1);
            if((cell=='#')&&(cellu=='#')&&(celld=='#')&&(celll!='#')&&(cellr!='#')){
                Vector2 position1 = {TILE_SIZE*(x+1),TILE_SIZE* y};
                Rectangle frame_torch_right={ sta->frame*11.0f, 29.0f, 11, 24};
                DrawTextureRec(lvl->texture_objects,frame_torch_right,position1,RAYWHITE);
                Vector2 position2 = {TILE_SIZE*x-11,TILE_SIZE* (y)};
                Rectangle frame_torch_left={ sta->frame*11.0f, 55.0f, 11, 24};
                DrawTextureRec(lvl->texture_objects,frame_torch_left,position2,RAYWHITE);                   
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
    draw_dynamic_objects(lvl,sta); // Added to draw dynamic objects (ISSUE 14)

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

    // Stop drawing relative to the camera
    EndMode2D();
}
