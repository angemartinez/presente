#include "load.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LINEA 100

void load_txt( char const *argv[],level *lvl){
	FILE* archivo = NULL;
	archivo = fopen(argv[0],"r");
	char* linea = NULL;
	int numLinea=1;
	int f,c;
	char* dimX = NULL;
    char* dimY = NULL;
    char *aux=NULL;
    int filas,columnas;

    dimX = malloc(2*sizeof(char));
    dimY = malloc(2*sizeof(char));
    linea = malloc(MAX_LINEA*sizeof(char));
    
    if(!archivo){
		printf("Error al abrir el archivo.\n");
        exit(-1);
	}
	f=0;
	fgets(linea,MAX_LINEA,archivo);
	while(!feof(archivo)){ // leemos el archivo y lo separamos
		if(numLinea==1){
			dimX= strtok(linea," \n");//separamos la linea
			columnas=atoi(dimX)*2; //asignamos el valor al numero de columnas y filas convirtiendolo en entero con atoi
			dimY=strtok(NULL," \n");
			filas=atoi(dimY);
			lvl->size_x=columnas;//asignamos los valores a lvl
			lvl->size_y=filas;
			lvl->cells=malloc(lvl->size_y*sizeof(char*)); // asignamos la memoria y llenamos con puntos
            for(int i=0; i<filas;i++){
                lvl->cells[i]=malloc(lvl->size_x*sizeof(char));
                for(int j=0;j<columnas;j++){
                    lvl->cells[i][j]= '.';
                }
			}
		}	
		if(numLinea>=2){// a
			c=0;
			for(int k=0; k<strlen(linea);k++){//reemplazamos los puntos por lo que corresponde el mapa leido
				if(linea[k] != ' '){
					aux=&linea[k];
					lvl->cells[f][k]=*aux;
					c++;
				}
			}
			f++;
		}
		fgets(linea,MAX_LINEA,archivo);
		numLinea++;
	}
	free(linea);
 	fclose(archivo);

 	const int screen_width = 800;
    const int screen_height = 600;

    InitWindow(screen_width,screen_height,"Presente - the game");
    SetTargetFPS(60);

    state *sta = malloc(sizeof(state));// iniciamos state
    memset(sta,0,sizeof(state));
    
    for(int x = 0; x < filas; x++){
        for(int y = 0; y< columnas; y++){
        	printf("%c", lvl->cells[x][y]);
            if(lvl->cells[x][y]!='p' || lvl->cells[x][y]!='b'||lvl->cells[x][y]!='b'||lvl->cells[x][y]!='#'||lvl->cells[x][y]!='.'){
                printf("Error, el archivo contiene caracteres que no corresponden\n");
                exit(-1);//corroboramos que el mapa leido tenga solo caracteres que correspondan
            }
            if (lvl->cells[x][y]=='p'){ //al encontrar p, asigna la posicion al player
               	sta->pla.ent.x = (float)(y)*TILE_SIZE/2;
    			sta->pla.ent.y = (float)(x)*TILE_SIZE/2;
    			sta->pla.ent.rad = PLAYER_RAD;
    			sta->pla.ent.hp  = PLAYER_HP;
            }if(lvl->cells[x][y]=='b'){// al encontrar b asigna la posicion a un BRUTUS
            	enemy *new_enemy = &sta->enemies[sta->n_enemies];
                sta->n_enemies++;
                memset(new_enemy,0,sizeof(enemy));
                new_enemy->ent.x = (float)(y+0.5)*TILE_SIZE;
                new_enemy->ent.y = (float)(x+0.5)*TILE_SIZE;  
                new_enemy->kind   = BRUTE;
                new_enemy->ent.hp = BRUTE_HP;
                new_enemy->ent.rad = BRUTE_RAD;
            }if(lvl->cells[x][y]=='m'){//al encontrar m asigna la posicion a un MINION
            	enemy *new_enemy = &sta->enemies[sta->n_enemies];
                sta->n_enemies++;
                memset(new_enemy,0,sizeof(enemy));
                new_enemy->kind   = MINION;
                new_enemy->ent.hp = MINION_HP;
                new_enemy->ent.rad = MINION_RAD;
                new_enemy->ent.x = (float)(y+0.5)*TILE_SIZE;
                new_enemy->ent.y = (float)(x+0.5)*TILE_SIZE;
            }    
        }
    }
    //Iniciamos el juego igual como en el main pero con los parámetros del mapa leido
    // We put the player in the center of the top-left cell.
    while(!WindowShouldClose()){

        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D);
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY()-GetScreenHeight()/2;
        float mouse_x = GetMouseX()-GetScreenWidth()/2;
        sta->aim_angle = atan2(-mouse_y,mouse_x);

        // Update the state
        state_update(lvl,sta);

        // Drawing
        BeginDrawing();

            ClearBackground(RAYWHITE);

            draw_state(lvl,sta);

            DrawText("Presente profe!",190,200,20,LIGHTGRAY);

        EndDrawing();

    }

    // Closer window
    CloseWindow();
    // Free memory
    exit(-1);
    
}   

























