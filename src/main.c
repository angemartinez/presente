
#include <raylib.h>
#include <math.h>

#include "level.h"
#include "draw.h"
#include "state.h"

int main(int argc, char const *argv[])
{

    // Initialization
    const int screen_width = 1024;
    const int screen_height = 768;
    float timer=0.0; // Timer for ISSUE 14
    int frame_n=0;  // frame number for ISSUE 14 (dynamic objects)

    InitWindow(screen_width, screen_height, "Presente - the game");
    SetTargetFPS(60);


    // Load Textures (ISSUE 14)
    Texture2D lvl_textures= LoadTexture("../resources/textures/Dungeon_Tileset.png"); // Texture for walls and floor
    Texture2D dyn_obj_textures= LoadTexture("../resources/textures/torch.png"); // Texture for dynamic objects
    // Load Music (EXTRA)
    InitAudioDevice();
    Music music = LoadMusicStream("../resources/audio/song.mp3");  // Music from Mario Bros
    Sound fxShoot = LoadSound("../resources/audio/shoot.wav");   // Sound for shooting
    PlayMusicStream(music);  // Start playins the music


    // Initialize level and fill randomly
    level *lvl = level_new(50,40);

    lvl->texture_lvl=lvl_textures; // Now lvl has texture_lvl and texture_objects
    lvl->texture_objects=dyn_obj_textures;

    level_fill_random(lvl,6);


    // Initialize state (and add enemies)
    state *sta = state_new();
    state_populate_random(lvl, sta, 50);


    // Main loop
    while(!WindowShouldClose()){
        UpdateMusicStream(music); // EXTRA plays the music.
        // Timer for ISSUE 14
        timer += GetFrameTime();
        if(timer >=0.2){
            timer=0;
            frame_n +=1;
        }
        sta->frame=frame_n%4; // Now sta has the number of frame from 0 to 3 (ISSUE 14)

        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D); // Es 1 si se presiona, 0 de lo contrario
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) PlaySound(fxShoot); // EXTRA, play the shoot sound when mouse left button is pressed

        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY() - GetScreenHeight() / 2;
        float mouse_x = GetMouseX() - GetScreenWidth() / 2;
        sta->aim_angle = atan2(-mouse_y, mouse_x);

        // Update the state
        state_update(lvl, sta);

        // Drawing
        BeginDrawing();

        ClearBackground(RAYWHITE);

        draw_state(lvl, sta);

        DrawText("Presente profe!", 100, 50, 20, LIGHTGRAY);

        EndDrawing();
    }
    //------------Unload textures music and sound, and close audio device -- ISSUE 14 and EXTRAS
    UnloadTexture(lvl_textures);
    UnloadTexture(dyn_obj_textures);
    UnloadMusicStream(music);
    UnloadSound(fxShoot);
    CloseAudioDevice();


    // Closer window
    CloseWindow();

    // Free memory
    state_free(sta);
    level_free(lvl);

    return 0;
}
