#include "ui.h"
#include "game.h"   // per avere Game, lives, score, energy
#include "resources.h"

void UI_Init(UI *ui)
{
    ui->barRect = (Rectangle){ 
        0, 
        PLAY_AREA_HEIGHT, 
        SCREEN_WIDTH, 
        UI_BAR_HEIGHT 
    };

    // UI rectangle image down to the right
    float hudWidth  = 160.0f;
    float padding   = 10.0f;

    ui->hudRightRect = (Rectangle){
        SCREEN_WIDTH - hudWidth - padding,
        PLAY_AREA_HEIGHT + padding,
        hudWidth,
        UI_BAR_HEIGHT - 2 * padding
    };

}

void UI_Draw(const UI *ui, const Game *game)
{
    // Draw Grey UI Rectangle 
    DrawRectangleRec(ui->barRect, (Color){ 80, 80, 80, 255 });

    // DEBUG to try the exact position
    //DrawRectangleLinesEx(ui->hudRightRect, 2, RED);

    // THere was many way to obtain this image: 1)use the cutted png image and put it on Resources.c or
    //2)cut it from the original atlas.In this case i had no problem with the background and i wanted to 
    //try this different way to obtain textures and sprites so i used the way for this image (2).
    Rectangle hudSrc = {
        102, 576,   // x, y in the atlas obtained thanks to : https://www.spritecow.com/
        278, 141    // width, height
    };

    DrawTexturePro(
        gAtlas,         // all textures images (atlas)
        hudSrc,            // rectangle part to obtain from atlas
        ui->hudRightRect,  // where draw it on UI
        (Vector2){0,0},    // origin
        0.0f,
        WHITE //maintains original color
    );
  
    // --- When start to draw all the info texts (lifes, score, energy) ---
    int baseX = 12;
    int baseY = (int)ui->barRect.y + 25;

    DrawText(TextFormat("  LIVES:"), baseX, baseY, 18, YELLOW);
    // life icons (taken from player png)
    float lifeIconScale = 0.45f;   // final dimention scale
    float iconW = game->player.size.x * lifeIconScale;
    float iconH = game->player.size.y * lifeIconScale;

    float lifeX = baseX+118;
    float lifeY = baseY - 5;   // little upper ENERGY text

    for (int i = 0; i < game->lives; i++)
    {
        Rectangle src = { 4, 400, 65, 65 };  // ←  player position in the player sheet thanks to : https://www.spritecow.com/
        Rectangle dest = {
            lifeX + i * (iconW + 5),  // space between life icons
            lifeY,
            iconW,
            iconH
        };

        DrawTexturePro(
            gPlayerSheet,   // use la sheet not animated
            src,
            dest,
            (Vector2){0,0},
            0,
            WHITE
        );
    }

    int energyY = baseY + 25;
    DrawText("SCORE:", baseX + 300, baseY, 18, YELLOW);
    DrawText(TextFormat("%d", game->score), baseX + 300, energyY, 18, RAYWHITE);

    
    DrawText("ENERGY:", baseX, energyY, 16, YELLOW);

    int barX = baseX + 90;
    int barY = energyY + 4;

    DrawRectangle(barX, barY, 150, 12, BLACK);
    DrawRectangle(barX, barY,
                  (int)(150 * (game->energy / 100.0f)),
                  12,
                  GREEN);
}
/*
void UI_Unload(UI *ui)
{
    if (ui->hasTexture) {
        //UnloadTexture(ui->hudRightTex);
        ui->hasTexture = 0;
        //UnloadTexture(ui->atlas);
    }
}*/
