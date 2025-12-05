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

//PRIVATE : Spawn ONE enemy //Not used for now
static void EnemySpawnOne(EnemyManager *mgr)
{
    Enemy *e = EnemyManagerGetFree(mgr);
    if (!e) return;  // no free slot

    // Random Enemy type[0..ENEMY_TYPE_COUNT-1]
    EnemyType type = (EnemyType)(rand() % ENEMY_TYPE_COUNT);

    //Setting texture
    Texture2D sheet = gEnemySheet[type];
    int frameCount = ENEMY_FRAME_COUNT;
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

    e->speed = ENEMY_BASIC_SPEED; // all the enemies on this speed
    //e->speed = ENEMY_BASIC_SPEED + game->score * 0.05f;   // aumenta con lo score

    // Animation init
    e->currentFrame = 0;
    e->frameTime    = ENEMY_FRAME_TIME;
    e->frameTimer   = 0.0f;

    //  Shooting: base interval + random component
    e->shootInterval = ENEMY_SHOOT_MIN_INTERVAL +  // tra ENEMY_SHOOT_MIN_INTERVAL e ENEMY_SHOOT_RANDOM_RANGE seconds
                        ((float)rand() / RAND_MAX) * ENEMY_SHOOT_RANDOM_RANGE;
    e->shootCooldown = e->shootInterval * ENEMY_SHOOT_INITIAL_RATIO;                   // little delay 
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

    e->active = 1;
    // use the same type for the current wave
    e->type = mgr->currentWaveType;

    // Set size from the enemy Sprite Sheet
    Texture2D sheet = gEnemySheet[e->type];
    int frameCount = ENEMY_FRAME_COUNT;
    float frameW = (float)sheet.width / frameCount;
    float frameH = (float)sheet.height;

    // init size ,position and velocity (before settings below)
    float w = 40.0f;
    float h = 40.0f;
    float x = 0.0f;
    float y = 0.0f;
    Vector2 vel = {0.0f, 1.0f}; // default can be leave like that: down

    switch (pattern) {
        case ENEMY_PATTERN_LEFT_DIAG_RIGHT:
            // start outsite from left
            x = -w - RandomFloat(ENEMY_SPAWN_OUT_MIN, ENEMY_SPAWN_OUT_MIN + ENEMY_SPAWN_OFFSET);
            // start a little outsite the upper part of the screen
            y = -h - RandomFloat(ENEMY_SPAWN_OUT_MIN, ENEMY_SPAWN_OUT_MAX);
            // going diagonal from left-up to right-down
            vel.x = RandomFloat(ENEMY_LEFT_DIAG_VEL_X_MIN, ENEMY_LEFT_DIAG_VEL_X_MAX);
            vel.y = RandomFloat(ENEMY_LEFT_DIAG_VEL_Y_MIN, ENEMY_LEFT_DIAG_VEL_Y_MAX);
            break;

        case ENEMY_PATTERN_RIGHT_DIAG_LEFT:
            // start outsite from right
            x = SCREEN_WIDTH + RandomFloat(ENEMY_SPAWN_OUT_MIN, ENEMY_SPAWN_OUT_MIN + ENEMY_SPAWN_OFFSET);
            y = -h - RandomFloat(ENEMY_SPAWN_OUT_MIN, ENEMY_SPAWN_OUT_MAX);
            // going diagonal from right-up to left-down
            vel.x = RandomFloat(ENEMY_RIGHT_DIAG_VEL_X_MIN, ENEMY_RIGHT_DIAG_VEL_X_MAX);
            vel.y = RandomFloat(ENEMY_RIGHT_DIAG_VEL_Y_MIN, ENEMY_RIGHT_DIAG_VEL_Y_MAX);
            break;

        case ENEMY_PATTERN_CENTER_DOWN:
        default:
            // more or les from up-center to down-center
            x = SCREEN_WIDTH / 2.0f + RandomFloat(-60.0f, 60.0f);
            y = -h - RandomFloat(ENEMY_SPAWN_OUT_MIN, ENEMY_SPAWN_OUT_MAX);
            vel.x = RandomFloat(ENEMY_CENTER_VEL_X_MIN, ENEMY_CENTER_VEL_X_MAX);
            vel.y = ENEMY_CENTER_VEL_Y;
            break;
    }

    e->position = (Vector2){ x, y };
    e->size     = (Vector2){ w, h };
    e->speed    = ENEMY_PATTERN_SPEED;//same for all types and patterns
    
    // Normalize direction function adn velocity set
    float len = sqrtf(vel.x*vel.x + vel.y*vel.y);
    if (len > 0.0001f) {
        vel.x /= len;
        vel.y /= len;
    }
    e->velocity = vel;

    // animation init
    e->currentFrame = 0;
    e->frameTime    = ENEMY_FRAME_TIME;
    e->frameTimer   = 0.0f;

    //Shooting: base interval + random component
    e->shootInterval = ENEMY_SHOOT_MIN_INTERVAL +
                       RandomFloat(0.0f, ENEMY_SHOOT_RANDOM_RANGE);
    e->shootCooldown = e->shootInterval * ENEMY_SHOOT_INITIAL_RATIO;
}


void EnemyManagerInit(EnemyManager *mgr)
{
   for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &mgr->enemies[i];
        e->active = 0;
        e->velocity = (Vector2){0, 1};
    }

    mgr->enemiesPerWave      = ENEMIES_PER_WAVE;     // n planes per wave
    mgr->delayBetweenEnemies = ENEMY_DELAY_BETWEEN_SPAWN; // 0.35s between one enemy spawn and the next one
    mgr->delayBetweenWaves   = ENEMY_DELAY_BETWEEN_WAVES;  // seconds from a wave to the next one
    mgr->spawnedInCurrentWave = 0;
    mgr->currentPattern       = ENEMY_PATTERN_LEFT_DIAG_RIGHT; //Initial pattern
    mgr->spawnTimer           = ENEMY_FIRST_SPAWN_DELAY;   // first wave init : after one second
    mgr->currentWaveType = (EnemyType)(rand() % ENEMY_TYPE_COUNT); // Initial Type
}

void EnemyManagerUpdate(EnemyManager *mgr, float dt)
{
   //  Movement update + death out of screen
    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &mgr->enemies[i];
        if (!e->active) continue;

        // start from a linear movement and apply velocity(thats its defined random horizontal and down movement in vertical)
        e->position.x += e->velocity.x * e->speed * dt;
        e->position.y += e->velocity.y * e->speed * dt;

        // if too much down out of screen, deactivate enemy
        if (e->position.y > PLAY_AREA_HEIGHT + e->size.y ||
            e->position.x < -e->size.x - ENEMY_SIDE_EXTRA ||
            e->position.x > SCREEN_WIDTH + ENEMY_SIDE_EXTRA)
        {
            e->active = 0;
        }

        // animation
        e->frameTimer += dt;
        if (e->frameTimer >= e->frameTime) {
            e->frameTimer = 0.0f;
            e->currentFrame = (e->currentFrame + 1) % ENEMY_FRAME_COUNT; // es: 3 frame
        }

        // shooting: cooldown upgrade
        if (e->shootCooldown > 0.0f) {
            e->shootCooldown -= dt;
        }
    }

    // Manage wave
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

        //Texture settings before the draw
        Texture2D sheet = gEnemySheet[e->type];
        int frameCount = ENEMY_FRAME_COUNT;
        float frameW = (float)sheet.width / frameCount;
        float frameH = (float)sheet.height;

        // Phase 1: here calculate the rect posizion and size calculation on the play area that i need for draw in the fuction
        Rectangle src = {
            frameW * e->currentFrame, // 0, W, 2W
            0.0f,
            frameW,
            frameH
        };

        // Phase 2: here calculate the frame to obtain from sprite sheet with width/3 (3 frames)
        Rectangle dest = {
            e->position.x,
            e->position.y,
            e->size.x,
            e->size.y
        };

        // Phase 2.5: here calculate rotation angle and the rotation origin
        // Calculate velocity angle
        // here we use fuction atan2f(y, x) return angle from x axis , in radiants
        float angleDeg = atan2f(e->velocity.y, e->velocity.x) * 180.0f / PI;
        // basically the sprite is drawn from down to up
        angleDeg += ENEMY_ROTATION_OFFSET;   // o -90.0f this set the base rotation from up to donw
        // origin at center of destination rectangle (for the rotation)
        Vector2 origin = {
            e->size.x / 2.0f,
            e->size.y / 2.0f
        };

        // Phase3 : Using the Draw fuction to Draw the player , with rotation
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
