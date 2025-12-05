// =========================================================
// Thanks to GEMINI : FOUND THIS DEFINE TO EXCLUDE windows api that caused "redefinition" error on Cmake compilation
// To use before every include that can contain
// the include of windows.h (like clove-unit.h  IMPLEMENTATION).Have to study more about this issue.
//TODO : ^^^^^^
// =========================================================
// MACRO WINDOWS (OK)
// =========================================================
#define NOUSER
#define NOMINMAX
#define NOGDI
#define WIN32_LEAN_AND_MEAN

// =========================================================
// my headers (depends on raylib)
// =========================================================
#include "player.h"
#include "config.h"
#include "bullet.h"
#include "enemy.h"

#define CLOVE_IMPLEMENTATION 
#include "clove-unit.h" 
//found on clove-unit this guard and i used here to creare a clove_TRUE function
#define CLOVE_IS_TRUE(res)  do { __CLOVE_ASSERT_GUARD __clove_assert_bool(__CLOVE_ASSERT_EQ, true, res, _this); } while(0)
#define CLOVE_SUITE_NAME GameTests



CLOVE_TEST(PlayerInit_DefaultValues) {
    Player p;
    PlayerInit(&p);

    CLOVE_INT_EQ(1, p.isAlive);
    CLOVE_FLOAT_EQ(220.0f, p.speed);
}


// Se usi Raylib nei test, includi raylib e inizializza un contesto minimo.
// Per ora testiamo solo parti che NON chiamano funzioni raylib, oppure
// lo facciamo in modo indiretto.



// --------- PLAYER TESTS ---------

CLOVE_TEST(PlayerInit_valuesPlayer2) {
    Player p;
    PlayerInit(&p);

    // size dal tuo codice (65x65)
    CLOVE_INT_EQ(65, (int)p.size.x);
    CLOVE_INT_EQ(65, (int)p.size.y);

    // dentro la play area (non testiamo valore esatto, solo il range)
    CLOVE_IS_TRUE(p.position.x >= 0);
    CLOVE_IS_TRUE(p.position.y >= 0);
    CLOVE_IS_TRUE(p.position.x + p.size.x <= SCREEN_WIDTH);
    CLOVE_IS_TRUE(p.position.y + p.size.y <= PLAY_AREA_HEIGHT);

    CLOVE_INT_EQ(1, p.isAlive);
    CLOVE_FLOAT_EQ(0.0f, p.invincibleTime);
    CLOVE_INT_EQ(0, p.isStunned);

    CLOVE_FLOAT_EQ(0.17f, p.shootInterval);
    CLOVE_FLOAT_EQ(0.0f, p.shootCooldown);
}


// simulate invicibility /stun
CLOVE_TEST(PlayerUpdate_InvincibilityExpiresAndUnstun) {
    Player p;
    PlayerInit(&p);

    p.invincibleTime = 1.0f;
    p.isStunned = 1;

    float dt = 0.5f;
    PlayerUpdate(&p, dt);      // t = 0.5 ancora invincibile
    CLOVE_FLOAT_EQ(0.5f, p.invincibleTime);
    CLOVE_INT_EQ(1, p.isStunned);

    PlayerUpdate(&p, dt);      // t = 1.0 finita invincibilità, stun rimosso
    CLOVE_FLOAT_EQ(0.0f, p.invincibleTime);
    CLOVE_INT_EQ(0, p.isStunned);
}

// BULLET MANAGER TESTS
CLOVE_TEST(BulletManager_Init_AllInFreeList) {
    BulletManager mgr;
    BulletManagerInit(&mgr);

    // activeList mus be empty
    CLOVE_NULL(mgr.activeList);

    //count how many bullet in the freeList
    Bullet *b = mgr.freeList;
    int count = 0;
    while (b) {
        count++;
        b = b->next;
    }

    CLOVE_INT_EQ(MAX_BULLETS, count);
}
/*
CLOVE_TEST(BulletManager_SpawnPlayer_DequeuesFromFreeList) {
    BulletManager mgr;
    BulletManagerInit(&mgr);

    int freeBefore = 0;
    Bullet *b = mgr.freeList;
    while (b) { freeBefore++; b = b->next; }

    Vector2 pos = {100.0f, 100.0f};
    Vector2 dir = {0.0f, -1.0f};

    BulletManager_SpawnPlayer(&mgr, pos, dir);

    // new free-count
    int freeAfter = 0;
    b = mgr.freeList;
    while (b) { freeAfter++; b = b->next; }

    // one less in  the freeList
    CLOVE_INT_EQ(freeBefore - 1, freeAfter);
    // one in the activeList
    CLOVE_NOT_NULL(mgr.activeList);
    CLOVE_TRUE(mgr.activeList->active);
    CLOVE_INT_EQ(BULLET_OWNER_PLAYER, mgr.activeList->owner);
    CLOVE_FLOAT_EQ(400.0f, mgr.activeList->speed);
    CLOVE_FLOAT_EQ(34.0f, mgr.activeList->damage);

    // direction Normalized (0,-1)
    CLOVE_FLOAT_EQ(0.0f, mgr.activeList->velocity.x);
    CLOVE_FLOAT_EQ(-1.0f, mgr.activeList->velocity.y);
}*/
// Bullet out of screen (have to be removed)
CLOVE_TEST(BulletManager_Update_RemovesOutOfBounds) {
    BulletManager mgr;
    BulletManagerInit(&mgr);

    Vector2 pos = {10.0f, -100.0f}; // se this is surely out of screen
    Vector2 dir = {0.0f, -1.0f};
    BulletManager_SpawnPlayer(&mgr, pos, dir);

    CLOVE_NOT_NULL(mgr.activeList);

    BulletManagerUpdate(&mgr, 0.016f); // dt dont mind here

    // have to return inside freeList and activeList must be NULL
    CLOVE_NULL(mgr.activeList);

    // check that there are MAX_BULLETS elements int freeList 
    int freeCount = 0;
    Bullet *b = mgr.freeList;
    while (b) { freeCount++; b = b->next; }
    CLOVE_INT_EQ(MAX_BULLETS, freeCount);
}



//ENEMY MANAGER TESTS
// Spawn time of the enemy test
CLOVE_TEST(EnemyManager_Update_SpawnsEnemiesOverTime) {
    EnemyManager mgr;
    EnemyManagerInit(&mgr);

    float totalTime = 0.0f;
    // update for some seconds
    while (totalTime < 5.0f) {
        EnemyManagerUpdate(&mgr, 0.1f);
        totalTime += 0.1f;
    }

    int activeCount = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (mgr.enemies[i].active) activeCount++;
    }

    // at least 1 shoul be present as active
    CLOVE_IS_TRUE(activeCount > 0);
}

//TODO: to verify better
CLOVE_TEST(EnemyManager_Init_DefaultWaveParams) {
    EnemyManager mgr;
    EnemyManagerInit(&mgr);

    // all not active
    for (int i = 0; i < MAX_ENEMIES; i++) {
        CLOVE_INT_EQ(0, mgr.enemies[i].active);
        CLOVE_FLOAT_EQ(0.0f, mgr.enemies[i].velocity.x);
        CLOVE_FLOAT_EQ(1.0f, mgr.enemies[i].velocity.y);
    }

    CLOVE_INT_EQ(7, mgr.enemiesPerWave);
    CLOVE_FLOAT_EQ(0.35f, mgr.delayBetweenEnemies);
    CLOVE_FLOAT_EQ(2.0f, mgr.delayBetweenWaves);

    // at list one active
    CLOVE_IS_TRUE(mgr.currentWaveType >= 0);
    CLOVE_IS_TRUE(mgr.currentWaveType < ENEMY_TYPE_COUNT);
}


//MAIN CLOVE RUNNER
CLOVE_RUNNER();
