#ifndef ISLAND_H
#define ISLAND_H

#include "raylib.h"
#include "config.h"

#define MAX_ISLANDS 8

typedef struct Island {
    int active;
    Texture2D *tex;
    Vector2 position;
    Vector2 size;
} Island;

typedef struct IslandManager {
    Island islands[MAX_ISLANDS];

    float spawnTimer;      // countdown to the next island spawn
    float minSpawnTime;    // minimun time to next island spawn
    float maxSpawnTime;    // max time to next..
} IslandManager;

void IslandManagerInit(IslandManager *mgr);
void IslandManagerUpdate(IslandManager *mgr, float dt, float scrollSpeed);
void IslandManagerDraw(const IslandManager *mgr);

#endif
