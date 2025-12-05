#include "enemy.h"
#include "resources.h"
#include <stdlib.h>   // rand
#include <time.h>     // mabye can seed on MAin
#include <math.h>

// Random float from min and max
static float RandomFloat(float min, float max)
{
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

// PRIVATE:Get Free Enemy from array
//TO DO: CHANGE ARRAY TO LIST
static Enemy* EnemyManagerGetFree(EnemyManager *mgr)
{
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!mgr->enemies[i].active) {
            return &mgr->enemies[i];
        }
    }
    return NULL;
}

//PRIVATE : Spawn ONE enemy
static void EnemySpawnOne(EnemyManager *mgr)
{
    Enemy *e = EnemyManagerGetFree(mgr);
    if (!e) return;  // no free slot

    // Random Enemy type[0..ENEMY_TYPE_COUNT-1]
    EnemyType type = (EnemyType)(rand() % ENEMY_TYPE_COUNT);

    Texture2D sheet = gEnemySheet[type];
    int frameCount = 3;

    float frameW = (float)sheet.width / frameCount;
    float frameH = (float)sheet.height;

    e->active = 1;
    e->type   = type;

    // natural dimention
    e->size = (Vector2){ frameW, frameH };

    // Position X random, this one no out of the window
    float xMin = 0.0f;
    float xMax = (float)SCREEN_WIDTH - e->size.x;
    float x = xMin + (float)rand() / (float)RAND_MAX * (xMax - xMin);

    e->position.x = x;
    e->position.y = -e->size.y;  // just a little upper the window screen

    e->speed = 190.0f; // all the enemies on this speed
//e->speed = 80.0f + game->score * 0.05f;   // aumenta col tempo

    // Animation
    e->currentFrame = 0;
    e->frameTime    = 0.1f;
    e->frameTimer   = 0.0f;

    //  Shooting: base interval + random component
    e->shootInterval = 0.5f + ((float)rand() / RAND_MAX) * 1.5f;  // tra 1.0 e 2.5 secondi
    e->shootCooldown = e->shootInterval * 0.3f;                   // un piccolo delay iniziale
    // TODO:
    //new bullet patterns and speeds(?)
}
//PRIVATE SPAWN ENEMY with Pattern
static void EnemyManager_SpawnOneWithPattern(EnemyManager *mgr, EnemyPattern pattern)
{
    //Find free slot
    Enemy *e = NULL;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!mgr->enemies[i].active) {
            e = &mgr->enemies[i];
            break;
        }
    }
    if (!e) return; // no free slot


     // Random Enemy type[0..ENEMY_TYPE_COUNT-1]
    EnemyType type = (EnemyType)(rand() % ENEMY_TYPE_COUNT);
    e->type   = type;


    e->active = 1;

    // use the same type for the current wave
    e->type = mgr->currentWaveType;

    // original size from the enemy Sprite Sheet
    Texture2D sheet = gEnemySheet[e->type];
    int frameCount = 3;
    float frameW = (float)sheet.width / frameCount;
    float frameH = (float)sheet.height;

    // init size and position (before settings below)
    float w = 40.0f;
    float h = 40.0f;

    float x = 0.0f;
    float y = 0.0f;
    Vector2 vel = {0.0f, 1.0f}; // default: down

    switch (pattern) {
        case ENEMY_PATTERN_LEFT_DIAG_RIGHT:
            // start outsite from left
            x = -w - RandomFloat(10.0f, 40.0f);
            // start a little outsite the upper part of the screen
            y = -h - RandomFloat(10.0f, 80.0f);
            // going diagonal from left-up to right-down
            vel.x = RandomFloat(0.4f, 0.7f);
            vel.y = RandomFloat(0.7f, 1.0f);
            break;

        case ENEMY_PATTERN_RIGHT_DIAG_LEFT:
            // start outsite from right
            x = SCREEN_WIDTH + RandomFloat(10.0f, 40.0f);
            y = -h - RandomFloat(10.0f, 80.0f);
            // going diagonal from right-up to left-down
            vel.x = RandomFloat(-0.7f, -0.4f);
            vel.y = RandomFloat(0.7f, 1.0f);
            break;

        case ENEMY_PATTERN_CENTER_DOWN:
        default:
            // more or les from up-center to down-center
            x = SCREEN_WIDTH / 2.0f + RandomFloat(-60.0f, 60.0f);
            y = -h - RandomFloat(10.0f, 80.0f);
            vel.x = RandomFloat(-0.2f, 0.2f);
            vel.y = 1.0f;
            break;
    }

    e->position = (Vector2){ x, y };
    e->size     = (Vector2){ w, h };
    e->speed    = 150.0f;
    //TODO check
    // Normalize direction
    float len = sqrtf(vel.x*vel.x + vel.y*vel.y);
    if (len > 0.0001f) {
        vel.x /= len;
        vel.y /= len;
    }
    e->velocity = vel;

    // animation
    e->currentFrame = 0;
    e->frameTime    = 0.1f;
    e->frameTimer   = 0.0f;

    //Shooting: base interval + random component
    e->shootInterval = 2.0f + RandomFloat(0.0f, 1.5f);
    e->shootCooldown = e->shootInterval * 0.5f;
}


void EnemyManagerInit(EnemyManager *mgr)
{
    //old
    /*
    for (int i = 0; i < MAX_ENEMIES; i++) {
        mgr->enemies[i].active = 0;
    }
    mgr->spawnTimer   = RandomFloat(0.18f, 2.0f);
    
    mgr->spawnInterval = 1.1f;   // uno ogni 1.5 secondi (regola tu)
    */
   for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &mgr->enemies[i];
        e->active = 0;
        e->velocity = (Vector2){0, 1};
    }

    mgr->enemiesPerWave      = 5;     // 5 planes per wave
    mgr->delayBetweenEnemies = 0.35f; // 0.35s between one enemy spawn and the next one
    mgr->delayBetweenWaves   = 2.0f;  // 2 secondi from a wave to the next one
    mgr->spawnedInCurrentWave = 0;
    mgr->currentPattern       = ENEMY_PATTERN_LEFT_DIAG_RIGHT;
    mgr->spawnTimer           = 1.0f;   // first wave init : after one second
    // TODO:
    //pre -defined patterns(?)
    mgr->currentWaveType = (EnemyType)(rand() % ENEMY_TYPE_COUNT); // 👈 tipo iniziale
}

void EnemyManagerUpdate(EnemyManager *mgr, float dt)
{
   // 1) movement update + death out of screen
    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &mgr->enemies[i];
        if (!e->active) continue;

        // start from a linear movement and apply velocity(thats its defined random horizontal and down movement in vertical)
        e->position.x += e->velocity.x * e->speed * dt;
        e->position.y += e->velocity.y * e->speed * dt;

        // if too much down out of screen, deactivate enemy
        if (e->position.y > PLAY_AREA_HEIGHT + e->size.y ||
            e->position.x < -e->size.x - 50 ||
            e->position.x > SCREEN_WIDTH + 50)
        {
            e->active = 0;
        }

        // animation
        e->frameTimer += dt;
        if (e->frameTimer >= e->frameTime) {
            e->frameTimer = 0.0f;
            e->currentFrame = (e->currentFrame + 1) % 3; // es: 3 frame
        }

        // shooting: cooldown upgrade
        if (e->shootCooldown > 0.0f) {
            e->shootCooldown -= dt;
        }
    }

    // 2) Manage wave
    mgr->spawnTimer -= dt;
    if (mgr->spawnTimer > 0.0f) {
        return;
    }

    if (mgr->spawnedInCurrentWave < mgr->enemiesPerWave) {
        // spawn new enemy in the current wave
        EnemyManager_SpawnOneWithPattern(mgr, mgr->currentPattern);
        mgr->spawnedInCurrentWave++;

        // set next enemy spawn time in the current wave to X seconds after
        mgr->spawnTimer = mgr->delayBetweenEnemies;
    }
    else {
        // wave end → choose new pattern for the way and reset main values
        mgr->spawnedInCurrentWave = 0;

        int p = rand() % 3;  // 3 pattern (0,1 or 2)
        mgr->currentPattern = (EnemyPattern)p;
         mgr->currentWaveType = (EnemyType)(rand() % ENEMY_TYPE_COUNT);  // random new type(texture for this wave)
        // pause time before new wave
        mgr->spawnTimer = mgr->delayBetweenWaves;
    }
}

void EnemyManagerDraw(const EnemyManager *mgr)
{
    for (int i = 0; i < MAX_ENEMIES; i++) {
        const Enemy *e = &mgr->enemies[i];
        if (!e->active) continue;

        Texture2D sheet = gEnemySheet[e->type];
        int frameCount = 3;

        float frameW = (float)sheet.width / frameCount;
        float frameH = (float)sheet.height;

        Rectangle src = {
            frameW * e->currentFrame, // 0, W, 2W
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

        // Calculate velocity angle
        // atan2f(y, x) return angle from x axis , in radiants
        float angleDeg = atan2f(e->velocity.y, e->velocity.x) * 180.0f / PI;

        // basically the sprite is drawn from down to up
        angleDeg += -90.0f;   // o -90.0f this set the base rotatio from up to donw

        // origine at center of destination rectangle
        Vector2 origin = {
            e->size.x / 2.0f,
            e->size.y / 2.0f
        };

        DrawTexturePro(
            sheet,
            src,
            (Rectangle){
                dest.x + origin.x,
                dest.y + origin.y,
                dest.width,
                dest.height
            },
            origin,
            angleDeg,
            WHITE
        );

    }
}
