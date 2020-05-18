#include "entity.h"
#include <math.h>
#include <stdio.h>



int entity_physics(const level *lvl, entity *ent){
    

    // EXTRA START OF ISSUE 12 ----------------------

    int old_x = (int) floor(ent->x/TILE_SIZE); // saves the old x and y pos.
    int old_y = (int) floor(ent->y/TILE_SIZE); 
    float circ_x; // creates a point to look for
    float circ_y;

    // depending of the velocity, consider the rad.
    // example if vx > 0 mens the entity is moving to the right, so add the radious to the point 
    // that we are going to look if is # or .
    if (ent->vx>0){
        circ_x=ent->x+ent->rad;
    }else if (ent->vx<0){
        circ_x=ent->x-ent->rad;
    }else{
        circ_x=ent->x;
    }
    
    if (ent->vy>0){
        circ_y=ent->y+ent->rad;
    }else if (ent->vy<0){
        circ_y=ent->y-ent->rad;
    }else{
        circ_y=ent->y;
    }


    int tile_x = (int) floor(circ_x/TILE_SIZE);
    int tile_y = (int) floor(circ_y/TILE_SIZE);
    // checks nearby cells in x,y and diag pos. 
    char cellx = level_get(lvl,tile_x,old_y);
    char celly = level_get(lvl,old_x,tile_y);
    char cellxy= level_get(lvl,tile_x,tile_y);

    // Depending if is # or ., add the velocity or not (update the pos or not)
    if((cellx=='#')&&(celly!='#')){
        ent->y += ent->vy;
        ent->x = ent->x;
        return 1;
    }else if((celly=='#')&&(cellx!='#')){
        ent->y = ent->y;
        ent->x += ent->vx;
        return 1;
    }else if((cellx=='#')&&(celly=='#')){
        ent->y = ent->y;
        ent->x = ent->x;        
        return 1;
    }else if (cellxy=='#'){
        ent->y = ent->y;
        ent->x = ent->x;        
        return 1;
    }else{
        ent->x += ent->vx;
        ent->y += ent->vy;       
        return 0;
    }
    // ---END OF ISSUE 12-----------------------------------
}

int entity_collision(const entity *ent1, const entity *ent2){
    // If one of the enitities is dead, there is no collision
    if(ent1->dead || ent2->dead) return 0;

    // Compute the distance between the two centers
    float delta_x = ent1->x - ent2->x;
    float delta_y = ent1->y - ent2->y;
    float delta_mag = sqrt(delta_x*delta_x+delta_y*delta_y);

    // Return 1 if the distance is smaller than the sum of both radious
    return (delta_mag < ent1->rad + ent2->rad);
}
