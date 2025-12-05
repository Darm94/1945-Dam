#include "explosion.h"
#include "resources.h"
#include <stdio.h>

static Explosion* GetFreeExplosion(ExplosionManager *mgr)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!mgr->explosions[i].active) return &mgr->explosions[i];
    }
    return NULL;
}

void ExplosionManagerInit(ExplosionManager *mgr)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        mgr->explosions[i].active = 0;
    }
}

void ExplosionManager_SpawnEnemy(ExplosionManager *mgr, Vector2 pos, Vector2 size)
{
    Explosion *e = GetFreeExplosion(mgr);
    if (!e) return;

    e->active = 1;
    e->type   = EXPLOSION_ENEMY;

    e->position = pos;
    e->size     = size;

    e->currentFrame = 0;
    e->frameCount   = 6;       // 6 strip enemy
    e->frameTime    = 0.06f;   // time between frames
    e->frameTimer   = 0.0f;
}

void ExplosionManager_SpawnPlayer(ExplosionManager *mgr, Vector2 pos, Vector2 size)
{
    Explosion *e = GetFreeExplosion(mgr);
    if (!e) return;

    e->active = 1;
    e->type   = EXPLOSION_PLAYER;

    e->position = pos;
    e->size     = size;

    e->currentFrame = 0;
    e->frameCount   = 7;       // 7 strip player
    e->frameTime    = 0.07f;   //time between frames
    e->frameTimer   = 0.0f;
}

void ExplosionManagerUpdate(ExplosionManager *mgr, float dt)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        Explosion *e = &mgr->explosions[i];
        if (!e->active) continue;

        e->frameTimer += dt;
        if (e->frameTimer >= e->frameTime) {
            e->frameTimer -= e->frameTime;
            e->currentFrame++;
            if (e->currentFrame >= e->frameCount) {
                e->active = 0;  // explosion end
            }
        }
    }
}

void ExplosionManagerDraw(const ExplosionManager *mgr)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        const Explosion *e = &mgr->explosions[i];
        if (!e->active) continue;

        Texture2D sheet;
        int frameCount = e->frameCount;

        if (e->type == EXPLOSION_ENEMY) {
            sheet = gExplosionEnemySheet;
        } else {
            sheet = gExplosionPlayerSheet;
        }

        float frameW = (float)sheet.width / frameCount;
        float frameH = (float)sheet.height;

        Rectangle src = {
            frameW * e->currentFrame,
            0.0f,
            frameW,
            frameH
        };

        Rectangle dest = {
            e->position.x,
            e->position.y,
            e->size.x,
            e->size.y
        };

        DrawTexturePro(
            sheet,
            src,
            dest,
            (Vector2){0,0},
            0.0f,
            WHITE
        );
    }
}
