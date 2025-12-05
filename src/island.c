#include "island.h"
#include "resources.h"
#include <stdlib.h>
#include <math.h>

static float RandomFloat(float min, float max)
{
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

void IslandManagerInit(IslandManager *mgr)
{
    for (int i = 0; i < MAX_ISLANDS; i++) {
        mgr->islands[i].active = 0;
        mgr->islands[i].tex = NULL;
    }

    mgr->minSpawnTime = 2.0f;
    mgr->maxSpawnTime = 5.0f;
    mgr->spawnTimer   = RandomFloat(mgr->minSpawnTime, mgr->maxSpawnTime);
}

// controlla se una nuova isola è troppo vicina alle altre
static int IslandManager_CanSpawnHere(IslandManager *mgr, float x, float y, float minDist)
{
    for (int i = 0; i < MAX_ISLANDS; i++) {
        Island *isl = &mgr->islands[i];
        if (!isl->active) continue;

        float dx = (isl->position.x + isl->size.x/2.0f) - x;
        float dy = (isl->position.y + isl->size.y/2.0f) - y;
        float dist = sqrtf(dx*dx + dy*dy);

        if (dist < minDist) return 0;
    }
    return 1;
}

void IslandManagerUpdate(IslandManager *mgr, float dt, float scrollSpeed)
{
    // movimento isole (stessa velocità del mare: verso l'alto o verso il basso in base al tuo segno)
    for (int i = 0; i < MAX_ISLANDS; i++) {
        Island *isl = &mgr->islands[i];
        if (!isl->active) continue;

        isl->position.y += scrollSpeed * dt;   // 👈 ORA vanno GIÙ

        // se esce dal fondo dell'area di gioco, disattiva
        if (isl->position.y > PLAY_AREA_HEIGHT) {
            isl->active = 0;
        }
    }
    // gestione spawn
    mgr->spawnTimer -= dt;
    if (mgr->spawnTimer > 0.0f) return;

    // reset timer per il prossimo spawn
    mgr->spawnTimer = RandomFloat(mgr->minSpawnTime, mgr->maxSpawnTime);

    // trova uno slot libero
    Island *slot = NULL;
    for (int i = 0; i < MAX_ISLANDS; i++) {
        if (!mgr->islands[i].active) {
            slot = &mgr->islands[i];
            break;
        }
    }
    if (!slot) return; // tutte occupate

    // scegli una delle 3 texture a caso
    Texture2D *texArr[3] = { &gIsland1, &gIsland2, &gIsland3 };
    int r = rand() % 3;
    Texture2D *chosen = texArr[r];

    float iw = (float)chosen->width;
    float ih = (float)chosen->height;

    // random X orizzontale, ma tenendo l'isola tutta dentro lo schermo
    float x = RandomFloat(0.0f, (float)SCREEN_WIDTH - iw);

    // Y sopra lo schermo (entra dall'alto)
float y = -ih - RandomFloat(10.0f, 60.0f);

    // controlla distanza minima (es. 120 pixel)
    if (!IslandManager_CanSpawnHere(mgr, x + iw/2.0f, y + ih/2.0f, 280.0f)) {
        return; // troppa vicinanza, saltiamo questo spawn
    }

    slot->active   = 1;
    slot->tex      = chosen;
    slot->position = (Vector2){ x, y };
    slot->size     = (Vector2){ iw, ih };
}

void IslandManagerDraw(const IslandManager *mgr)
{
    for (int i = 0; i < MAX_ISLANDS; i++) {
        const Island *isl = &mgr->islands[i];
        if (!isl->active || !isl->tex) continue;

        DrawTexture(*isl->tex,
                    (int)isl->position.x,
                    (int)isl->position.y,
                    WHITE);
    }
}
