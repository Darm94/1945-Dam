#include "bullet.h"
#include "resources.h"
#include <math.h>    // need it for sqrtf and atan2f

// =======================================================================
// Private API


// pop from freeList
static Bullet* BulletManager_GetFree(BulletManager *mgr)
{
    Bullet *b = mgr->freeList;
    if (b) {
        mgr->freeList = b->next;
        b->next = NULL;
    }
    return b;
}

// push in  activeList
static void BulletManager_AddActive(BulletManager *mgr, Bullet *b)
{
    b->next = mgr->activeList;
    mgr->activeList = b;
}

// rimove bullet from activeList
void BulletManager_RemoveActive(BulletManager *mgr, Bullet *b, Bullet *prev)
{
    if (prev) {
        prev->next = b->next;
    } else {
        mgr->activeList = b->next;
    }

    b->active = 0;

    // put again in freeList
    b->next = mgr->freeList;
    mgr->freeList = b;
}

// vector normalization from WEB : ( casuse seems to be a safety best practice)
static Vector2 NormalizeSafe(Vector2 v)
{
    float len = sqrtf(v.x*v.x + v.y*v.y);
    if (len > 0.0001f) {
        v.x /= len;
        v.y /= len;
    }
    return v;
}

// =======================================================================
// Public API


void BulletManagerInit(BulletManager *mgr)
{
    mgr->activeList = NULL;
    mgr->freeList   = NULL;

    // all bullets start in freeList
    for (int i = 0; i < MAX_BULLETS; i++) {
        Bullet *b = &mgr->pool[i];
        b->active = 0;
        b->next   = mgr->freeList;
        mgr->freeList = b;
    }
}

void BulletManagerUpdate(BulletManager *mgr, float dt)
{
    Bullet *prev = NULL;
    Bullet *b = mgr->activeList;

    while (b) {

        //  if during some function (collision in particular) put active = 0, we have to remove here for safety
        if (!b->active) {
            Bullet *toRemove = b;
            b = b->next;
            BulletManager_RemoveActive(mgr, toRemove, prev);
            continue;
        }
        
        // basic bullet movement: pos += vel * dt
        b->position.x += b->velocity.x * b->speed * dt;
        b->position.y += b->velocity.y * b->speed * dt;

        int kill = 0;
        // if goes out of window range , kill the bullet (just remove it from active)
        if (b->position.x + b->size.x < 0 ||
            b->position.x > SCREEN_WIDTH ||
            b->position.y + b->size.y < 0 ||
            b->position.y > PLAY_AREA_HEIGHT)  // PLAY AREA LIMIT
        {
            kill = 1;
        }

        // TODO: bullet max lifetime(?)
        if (kill) {
            Bullet *toRemove = b;
            b = b->next;
            BulletManager_RemoveActive(mgr, toRemove, prev);
            continue;
        }

        prev = b;
        b = b->next;
    }
}

void BulletManagerDraw(const BulletManager *mgr)
{
    Bullet *b = mgr->activeList;

    while (b) {
        // texture to draw dipends by player/enemy
        Texture2D tex;
        if (b->owner == BULLET_OWNER_PLAYER) {
            tex = gPlayerBulletTex;
        } else {
            tex = gEnemyBulletTex;
        }

         // Phase 1: here calculacte the rect posizion and size calculation on the play area that i need for draw in the fuction
        Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
        // Phase 2: here calculate the frame to obtain from sprite sheet with width/3 (3 frames)
        Rectangle dest = {
            b->position.x,
            b->position.y,
            b->size.x,
            b->size.y
        };
        // Phase 2.5: here calculate rotation angle and the rotation origin
        //TODO chek this part, need to test it
        // calculate actual bullet velocity vector rotation
        float angleDeg = atan2f(b->velocity.y, b->velocity.x) * 180.0f / PI +  BULLET_ROTATION_OFFSET;
   
        // origin from center of destination size
        Vector2 origin = { b->size.x / 2.0f, b->size.y / 2.0f };

        // Phase3 : Using the Draw fuction to Draw the player , no offsets and 0 rotation
        DrawTexturePro(
            tex,
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
        b = b->next;
    }
}

void BulletManager_SpawnPlayer(BulletManager *mgr, Vector2 pos, Vector2 dir)
{
    Bullet *b = BulletManager_GetFree(mgr);
    if (!b) return;  // pool is full

    dir = NormalizeSafe(dir);

    b->active = 1;
    b->owner  = BULLET_OWNER_PLAYER;

    b->position = pos;
    b->velocity = dir;
    b->speed    = PLAYER_BULLET_SPEED;   // basic bullet speed
    b->damage   = PLAYER_BULLET_DAMAGE;    // player bullet damage

    // bullet scale
    float scale = PLAYER_BULLET_SCALE;
    b->size = (Vector2){
        gPlayerBulletTex.width  * scale,
        gPlayerBulletTex.height * scale
    };
    //TODO: CHECK
    // center bullet based of precedent position
    b->position.x -= b->size.x / 2.0f;
    BulletManager_AddActive(mgr, b);
}

void BulletManager_SpawnEnemy(BulletManager *mgr, Vector2 pos, Vector2 dir)
{
    Bullet *b = BulletManager_GetFree(mgr);
    if (!b) return;

    dir = NormalizeSafe(dir);

    b->active = 1;
    b->owner  = BULLET_OWNER_ENEMY;

    b->position = pos;
    b->velocity = dir;
    b->speed    = ENEMY_BULLET_SPEED;   // enemy bullet  basic speed
    b->damage   = ENEMY_BULLET_DAMAGE;    // bullet damage to player

    float scale = ENEMY_BULLET_SCALE;     // enemy bullet scale
    b->size = (Vector2){
        gEnemyBulletTex.width  * scale,
        gEnemyBulletTex.height * scale
    };
    
    // center bullet based of precedent position
    b->position.x -= b->size.x / 2.0f;
    b->position.y -= b->size.y / 2.0f;
    BulletManager_AddActive(mgr, b);
}