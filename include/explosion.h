#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "raylib.h"
#include "config.h"

#define MAX_EXPLOSIONS 32

typedef enum ExplosionType {
    EXPLOSION_ENEMY = 0,
    EXPLOSION_PLAYER = 1
} ExplosionType;

typedef struct Explosion {
    int active;
    ExplosionType type;

    Vector2 position;
    Vector2 size;

    //Animation datas
    int   currentFrame;
    int   frameCount;
    float frameTime;   // time between frames
    float frameTimer; //time counter
} Explosion;

typedef struct ExplosionManager {
    Explosion explosions[MAX_EXPLOSIONS];
} ExplosionManager;

void ExplosionManagerInit(ExplosionManager *mgr);
void ExplosionManagerUpdate(ExplosionManager *mgr, float dt);
void ExplosionManagerDraw(const ExplosionManager *mgr);

// da chiamare quando qualcosa esplode:
void ExplosionManager_SpawnEnemy(ExplosionManager *mgr, Vector2 pos, Vector2 size);
void ExplosionManager_SpawnPlayer(ExplosionManager *mgr, Vector2 pos, Vector2 size);

#endif
