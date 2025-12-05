#include "raylib.h"
#include "game.h"   // se vuoi in futuro usare Game, ecc.

int main(void)
{
    const int screenWidth  = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "Test App - 1945 Core");
    InitAudioDevice();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        DrawText("Hello from TEST APP!", 100, 200, 20, RAYWHITE);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
