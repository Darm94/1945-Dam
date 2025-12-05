#include "resources.h"

// Choosed this resources 
Texture2D gAtlas = {0};
//Texture2D gUi= {0};

Texture2D gPlayerSheet = {0}; 
Texture2D gEnemySheet[3] = {0};
Texture2D gExplosionEnemySheet = {0};
Texture2D gExplosionPlayerSheet = {0};
Texture2D gPlayerBulletTex = {0};
Texture2D gEnemyBulletTex  = {0};
Texture2D gGameOverScreen = {0};
Texture2D gMenuScreen= {0};


//Background
Texture2D gWater = {0};
Texture2D gIsland1 = {0};
Texture2D gIsland2 = {0};
Texture2D gIsland3 = {0};
//Music and sound
Music gMusicBackground = {0};
Music gMenuBackground = {0};
Sound gExplosionEnemy  = {0};
Sound gExplosionPlayer = {0};
Sound gPlayerShootSound = {0};

void ResourcesLoad(void)
{
   
    //Setted on Cmake resource directory here: in bin/resources/
    gAtlas = LoadTexture("resources/1945_atlas.bmp");
    gWater = LoadTexture("resources/water.png");
    gPlayerSheet = LoadTexture("resources/player_sheet.png");

    gEnemySheet[0] = LoadTexture("resources/enemy1_sheet.png");
    gEnemySheet[1] = LoadTexture("resources/enemy2_sheet.png");
    gEnemySheet[2] = LoadTexture("resources/enemy3_sheet.png");
    gExplosionEnemySheet  = LoadTexture("resources/explosion_enemy.png");
    gExplosionPlayerSheet = LoadTexture("resources/explosion_player.png");
    gPlayerBulletTex = LoadTexture("resources/player_bullet.png");
    gEnemyBulletTex  = LoadTexture("resources/enemy_bullet.png");
    gGameOverScreen = LoadTexture("resources/game_over_screen.png");
    gMenuScreen = LoadTexture("resources/menu_image.png");

    gIsland1 = LoadTexture("resources/island1.png");
    gIsland2 = LoadTexture("resources/island2.png");
    gIsland3 = LoadTexture("resources/island3.png");

    // Music
    gMusicBackground = LoadMusicStream("resources/background.mp3");
    gMenuBackground = LoadMusicStream("resources/8bit_menu.mp3");
    SetMusicVolume(gMusicBackground, 0.25f);  // volume (0.0 - 1.0)

    // Esplosions and shoot sounds
    gExplosionEnemy  = LoadSound("resources/snd_explosion1.wav");
    gExplosionPlayer = LoadSound("resources/snd_explosion2.wav");
    gPlayerShootSound = LoadSound("resources/fire1.mp3");

    SetSoundVolume(gExplosionEnemy,  0.45f);  
    SetSoundVolume(gExplosionPlayer, 0.45f);
    SetSoundVolume(gPlayerShootSound, 0.25f);//bullet shoot less loud then rest cause it can be played very ofter
    SetSoundPitch(gPlayerShootSound,1.2); 
        
}


void ResourcesUnload(void)
{
    UnloadTexture(gAtlas);
    UnloadTexture(gWater);
    UnloadTexture(gPlayerSheet);

    UnloadMusicStream(gMusicBackground);
    UnloadSound(gExplosionEnemy);
    UnloadSound(gExplosionPlayer);
    UnloadSound(gPlayerShootSound);

    for (int i = 0; i < 3; i++)
        UnloadTexture(gEnemySheet[i]);
    UnloadTexture(gExplosionEnemySheet);
    UnloadTexture(gExplosionPlayerSheet);
    UnloadTexture(gPlayerBulletTex);
    UnloadTexture(gEnemyBulletTex);
    UnloadTexture(gGameOverScreen);

    UnloadTexture(gIsland1);
    UnloadTexture(gIsland2);
    UnloadTexture(gIsland3);
}
