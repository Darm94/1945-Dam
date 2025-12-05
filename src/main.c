#include "raylib.h"
#include "game.h"
#include "resources.h"

int main(void)
{
    // INIT WINDOW
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "1945 Demo - Damiano Schirru C");
    SetTargetFPS(60);
    const char* app_dir = GetApplicationDirectory();
    TraceLog(LOG_INFO,"Bin directory: %s",app_dir);
    ChangeDirectory(app_dir);

    //Png loaded and released here instantly
    Image icon = LoadImage("resources/icon.png");
    if (icon.data != NULL) {
        SetWindowIcon(icon);
        UnloadImage(icon);  //now we can free
    } else {
        TraceLog(LOG_WARNING, "Failed to load window icon!");
    }

    // AUDIO INIT
    // InitAudioDevice();
    InitAudioDevice();
    ResourcesLoad();// resource.h
    // Start only one Music song
    PlayMusicStream(gMenuBackground);
    gMenuBackground.looping = true;   // loop the song
    
    Game game;
    game.currentScreen = SCREEN_MAIN_MENU;
    GameInit(&game);
    
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // UPDATE
        GameUpdate(&game, dt);

        // DRAW
        GameDraw(&game);
    }

    GameUnload(&game);
    
    ResourcesUnload();


    CloseAudioDevice();//raylib ask to close the  audiodevice
    CloseWindow();
    

    return 0;
}
































































/*

#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    
    //Link Resources with bin file
    const char* app_dir = GetApplicationDirectory();
    TraceLog(LOG_INFO,"Bin directory: %s",app_dir);
    ChangeDirectory(app_dir);

    Texture2D guy = LoadTexture("resources/guy.png");
    if (guy.id == 0){
        TraceLog(LOG_ERROR,"Error loading guy png!");
        exit(-1);
    }
    
    //Font my_font..LoadFont
    InitAudioDevice();
    
    //--------------------------------------------------------------------------------------
    //Sound sound = LoadSound("pat to sound"); //tutto in memoria
    //SetSound o Set Music per i fari settaggi volume
    //Music music = LoadMusicStream("path to music");//la carica in streaming dal disco
    //music.looping =  true
    //PlayMusicStream(music);//play quando decidete voi
    char title[100];
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float delta_time = GetFrameTime();
        int fps = GetFPS();
        sprintf(title, "MY GAME: Delta Time: %.4f - FPS: %d", delta_time, fps);
        SetWindowTitle(title);

        static float time= 0;
        time += delta_time;
        unsigned char r = (unsigned char)(((sin(time)+1)*0.5f)* 255);
        Color clear = (Color){r,0,0,255};

        Vector2 mouse_pos=GetMousePosition();
        Color color = MAGENTA;
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            color =GREEN;
            //PlaySound(sound);
        }
*/
        /*if (IsMusicStreamPlaying(music)){
            UpdateMusicStream(music);

        }*/
        //PauseMusicStream(music);
        //ResumeMusicStream(music); oppure fai stop
        //verifica per aggiornamento musica
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
     /*  BeginDrawing();

            //ClearBackground(clear);
            ClearBackground(BLACK);
            //Font my_font = LoadFont("...ttf") e poi UNloadFont in fonto
            //e poi lo passate qua DrawTextEx()
            DrawCircleV(mouse_pos,20, color);

            DrawPixel(10,10,GREEN);
            DrawLine(100, 100, 700, 100, BLUE);
            DrawLineEx((Vector2){100,110},(Vector2){700,110},4,LIME);
            DrawRectangle(200,200,150,100,RED);
            DrawRectangleLines(400,200,150,100,PINK);
            //DrawRectanglesLines() ecc...
            DrawCircle(500,300,50,PURPLE);
            //DrawCircleLines
            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            //DrawTexture(guy,50,50,WHITE);
            //DrawTextureEx
            DrawTextureRec(guy, (Rectangle){0,0,300,300},(Vector2){0,0},WHITE);
            
            Rectangle source = (Rectangle){0,0,300,300};
            Rectangle dest = (Rectangle){0,0,(float)GetScreenWidth(),(float)GetScreenHeight()};
            Vector2 orig = (Vector2){0,0};
            DrawTexturePro(guy,source,dest,orig,0,WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    UnloadTexture(guy);
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
    */