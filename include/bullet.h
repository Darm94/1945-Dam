#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include "config.h"

#define MAX_BULLETS 128

typedef enum BulletOwner {
    BULLET_OWNER_PLAYER = 0,
    BULLET_OWNER_ENEMY  = 1
} BulletOwner;

typedef struct Bullet {
    int active;            // 0 = not used, 1 = "flying bullet"

    BulletOwner owner;

    Vector2 position;      
    Vector2 velocity;      // norm. directions * speed
    Vector2 size;          

    float speed;           
    float damage;          // enemy damage on player

    struct Bullet *next;   // decided to insert list logic directly in the bullet for an easer push/pull
} Bullet;

typedef struct BulletManager {
    Bullet pool[MAX_BULLETS];

    Bullet *activeList;    // active bullet list
    Bullet *freeList;      // free bullet list

    // TODO: counters
} BulletManager;

// Init / update / draw
void BulletManagerInit(BulletManager *mgr);
void BulletManagerUpdate(BulletManager *mgr, float dt);
void BulletManagerDraw(const BulletManager *mgr);

// Spawn API (to be used in game/player/enemies parts)
void BulletManager_SpawnPlayer(BulletManager *mgr, Vector2 pos, Vector2 dir);
void BulletManager_SpawnEnemy(BulletManager *mgr, Vector2 pos, Vector2 dir);
void BulletManager_RemoveActive(BulletManager *mgr, Bullet *b, Bullet *prev);

#endif
