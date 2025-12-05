#ifndef RESOURCES_H
#define RESOURCES_H

#include "raylib.h"

// ============
// TEXTURES
// ============
extern Texture2D gAtlas;        // my 1945_atlas.bmp
//extern Texture2D gUi;
extern Texture2D gWater; //mare
extern Texture2D gPlayerSheet;
extern Texture2D gEnemySheet[3];
extern Texture2D gExplosionEnemySheet;
extern Texture2D gExplosionPlayerSheet;
extern Texture2D gPlayerBulletTex;
extern Texture2D gEnemyBulletTex;
extern Texture2D gGameOverScreen;
extern Texture2D gMenuScreen;

extern Texture2D gIsland1;
extern Texture2D gIsland2;
extern Texture2D gIsland3;

extern Music gMusicBackground;
extern Music gMenuBackground;
extern Sound gExplosionEnemy;   // snd_explosion1.wav
extern Sound gExplosionPlayer;  // snd_explosion2.wav
extern Sound gPlayerShootSound;

//TODO: add new fonts

// load all this resources (texture/sound/fonts)
void ResourcesLoad(void);

// release all the resources
void ResourcesUnload(void);

#endif