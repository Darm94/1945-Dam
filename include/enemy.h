#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "config.h"
#include "enemy.h"

#define MAX_ENEMIES 32

typedef enum EnemyType {
    ENEMY_TYPE_0 = 0,
    ENEMY_TYPE_1 = 1,
    ENEMY_TYPE_2 = 2,
    ENEMY_TYPE_COUNT
} EnemyType;

typedef enum EnemyPattern {
    ENEMY_PATTERN_LEFT_DIAG_RIGHT = 0, // From Up-Light TO Down-right
    ENEMY_PATTERN_RIGHT_DIAG_LEFT = 1, // From Up-Right TO Down-left
    ENEMY_PATTERN_CENTER_DOWN     = 2  // From up-Center To Down-Center
    //TODO: add other pre-defined patterns or AI pattern logics
} EnemyPattern;

typedef struct Enemy {
    int active;           // 0 = not used, 1 = spawned and active

    EnemyType type;

    Vector2 position;
    Vector2 size;
    float speed;

    // Animation (3 frames horiz.)
    int   currentFrame;   // 0,1 or 2
    float frameTime;      // time between frames (es. 0.1)
    float frameTimer;     // frame timer
    // --- Shooting ---
    float shootCooldown;   // shoot countdown counter
    float shootInterval;   // shoot countdown max value
    // Direction value
    Vector2 velocity;   // dir * speed
} Enemy;

typedef struct EnemyManager {
    //TO Do: after this try , convert this to List as done in Bullet Manager
    Enemy enemies[MAX_ENEMIES];

    float spawnTimer;
    float spawnInterval; 

    // spawn a wave management
    //float spawnTimer;        
    int   enemiesPerWave;      
    int   spawnedInCurrentWave;// how many spawned in this wave
    float delayBetweenEnemies; // in the same wave
    float delayBetweenWaves;   // pause between wave and the next onee
    EnemyPattern currentPattern;

    EnemyType currentWaveType;  // i liked the idea to have the same ennemy type for every single wave
} EnemyManager;

void EnemyManagerInit(EnemyManager *mgr);
void EnemyManagerUpdate(EnemyManager *mgr, float dt);
void EnemyManagerDraw(const EnemyManager *mgr);

#endif
