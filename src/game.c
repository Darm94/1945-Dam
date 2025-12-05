#include "config.h"
#include "game.h"
#include "raylib.h"
#include "resources.h"
#include <math.h>
#include "bullet.h"
#include <stdlib.h>

//#include "collision.h"  //TODO

static void Game_HandlePlayerShooting(Game *game, float dt);
static void Game_HandleCollisions(Game *game);
static void Game_HandleEnemyShooting(Game *game);
static void Game_RespawnPlayer(Game *game);
static void Game_OnPlayerKilled(Game *game);

//First Game Function called at start
void GameInit(Game *game)
{
    game->score  = 0;
    game->lives  = 3;       // UI: life count
    game->energy = 100.0f;  // UI: energy bar
    game->waterScroll = 0.0f;//parte da 0 new

    PlayerInit(&game->player);
    EnemyManagerInit(&game->enemies);
    BulletManagerInit(&game->bullets); 
    ExplosionManagerInit(&game->explosions);
    UI_Init(&game->ui);
    IslandManagerInit(&game->islands);
}

void GameUpdate(Game *game, float dt)
{
    switch (game->currentScreen) {
        case SCREEN_GAMEPLAY:

            UpdateMusicStream(gMusicBackground);
            // ---  BACKGROUND SCROLL---
            float scrollSpeed = 60.0f;  // pixel for seconds
            game->waterScroll -= scrollSpeed * dt;  //  change direction with  + or -
            float tileH = (float)gWater.height;
            if (game->waterScroll <= 0.0f)
                game->waterScroll += tileH;   // loop between 0 e tileH
                
            // INPUT + PLAYER
            PlayerUpdate(&game->player, dt);
            BulletManagerUpdate(&game->bullets, dt);

            EnemyManagerUpdate(&game->enemies, dt);

            ExplosionManagerUpdate(&game->explosions, dt);
             // input shoot player
            Game_HandlePlayerShooting(game, dt); 
            Game_HandleEnemyShooting(game); 

            // collision
            Game_HandleCollisions(game);
            IslandManagerUpdate(&game->islands, dt, scrollSpeed);

            break;

        case SCREEN_GAMEOVER:
            // press enter go to main menu
            if (IsKeyPressed(KEY_ENTER)) {
                StopMusicStream(gMusicBackground);
                // reset e play new music
                SeekMusicStream(gMenuBackground, 0.0f);//play from the start
                PlayMusicStream(gMenuBackground);
                gMenuBackground.looping = true;
                GameInit(game); // reset all values
                game->currentScreen = SCREEN_MAIN_MENU;
            }
            break;
        case SCREEN_MAIN_MENU:
            // Custom main menu music
            UpdateMusicStream(gMenuBackground);
            // Enter for a new game 
            if (IsKeyPressed(KEY_ENTER)) {
                StopMusicStream(gMenuBackground);
                //TODO: to unify this music process in one fuction
                //reset e play new music (redundacy here)
                SeekMusicStream(gMusicBackground, 0.0f);
                PlayMusicStream(gMusicBackground);
                gMusicBackground.looping = true;
                GameInit(game); // reset all values
                game->currentScreen = SCREEN_GAMEPLAY;
            }
            break;
    }
}

void GameDraw(const Game *game)
{
    BeginDrawing();
    ClearBackground(DARKGRAY); // out of play area 

    switch (game->currentScreen) {
        case SCREEN_GAMEPLAY:

            //Game area height is PLAY_AREA_HEIGHT,the rest of the window height will be the UI bar
            //Scrolling water tile
            float tileW = (float)gWater.width;
            float tileH = (float)gWater.height;

            //FROM WEB BLOCK: ========================================================================
            // Vertical scrolling start point (scroll using tiles)
            //waterScroll -- = go the water screen go down
            float offsetY = fmodf(game->waterScroll, tileH);
            if (offsetY < 0) offsetY += tileH;

            // start from -tileH to cover all shortcomings
            for (float y = -offsetY; y < PLAY_AREA_HEIGHT; y += tileH)
            {
                for (float x = 0; x < SCREEN_WIDTH; x += tileW)
                {
                    DrawTexture(gWater, (int)x, (int)y, WHITE);
                }
            }
            //END FROM WEB BLOCK ======================================================================

            // Island over the water
            IslandManagerDraw(&game->islands);

            //In order Enemies,Bullets,Player and Explosions (that i want to cover the rest)
            EnemyManagerDraw(&game->enemies);
            BulletManagerDraw(&game->bullets);
            PlayerDraw(&game->player);
            ExplosionManagerDraw(&game->explosions);

            //UI bar
            UI_Draw(&game->ui, game);
            break;

        case SCREEN_GAMEOVER:
            // GAme Over image (over all the window)
            DrawTexturePro(
                gGameOverScreen,
                (Rectangle){0, 0, gGameOverScreen.width, gGameOverScreen.height},
                (Rectangle){
                    0,
                    0,
                    (float)SCREEN_WIDTH,
                    (float)SCREEN_HEIGHT
                },
                (Vector2){0,0},
                0.0f,
                WHITE
            );
            
            DrawText("GAME OVER", SCREEN_WIDTH/2 - 121, SCREEN_HEIGHT/2 - 250, 40, RED);
            DrawText(TextFormat("SCORE: %d", game->score),
             SCREEN_WIDTH/2 - 96,      
             SCREEN_HEIGHT/2 - 200,     // under GAME OVER text
             32,                        
             YELLOW);
            DrawText("PRESS ENTER TO RETURN TO MENU",
                 SCREEN_WIDTH/2 - 230,
                 SCREEN_HEIGHT - 80,
                 22,
                 RAYWHITE);
            break;
        case SCREEN_MAIN_MENU:
            //Image on all screen
            // GAme Over image (over all the window)
            DrawTexturePro(
                gMenuScreen,
                (Rectangle){0, 0, gGameOverScreen.width, gGameOverScreen.height},
                (Rectangle){
                    0,
                    0,
                    (float)SCREEN_WIDTH,
                    (float)SCREEN_HEIGHT
                },
                (Vector2){0,0},
                0.0f,
                WHITE
            );
            // easy monocolor background
            //ClearBackground(DARKBLUE);

            DrawText("STRIKERS 1945 DAMIANO EDITION",
                    SCREEN_WIDTH/2 - 232,
                    SCREEN_HEIGHT/2 -10,
                    28,
                    YELLOW);

            DrawText("PRESS ENTER TO START",
                    SCREEN_WIDTH/2 - 145,
                    SCREEN_HEIGHT/2 + 180,
                    24,
                    RAYWHITE);
            break;
    }

    EndDrawing();
}

void GameUnload(Game *game)
{
    //UI_Unload(&game->ui);
    //TODO:  put unload resources here?
}

//TODO: FUNCTION TO MOVE ON COLLISION.h ENEMY.h and PLAYER.h
static void Game_HandlePlayerShooting(Game *game, float dt)
{
    Player *p = &game->player;

    if (!p->isAlive) return;
    if (p->isStunned) return;

    // shoot cooldown
    if (p->shootCooldown > 0.0f) {
        p->shootCooldown -= dt;
    }

    // if space pressed and cooldown end -> shoot
    if (IsKeyDown(KEY_SPACE) && p->shootCooldown <= 0.0f) {
        p->shootCooldown = p->shootInterval;

        // bullet spawn point (player center)
        Vector2 spawnPos = { p->position.x + p->size.x / 2.0f,p->position.y};
        // bullet direction : up (0, -1)
        Vector2 dir = { 0.0f, -1.0f };

        BulletManager_SpawnPlayer(&game->bullets, spawnPos, dir);
        PlaySound(gPlayerShootSound);//bullet audio
    }
}
static void Game_HandleEnemyShooting(Game *game)
{
    EnemyManager *mgr = &game->enemies;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &mgr->enemies[i];
        if (!e->active) continue;

        if (e->shootCooldown <= 0.0f) {
            // random shoot chance
            float chance = (float)rand() / RAND_MAX;
            if (chance < 0.5f) {   // magic number for now : 50% shoot probability
                //bullet spawn position : enemy center & down part
                Vector2 spawnPos = {
                    e->position.x + e->size.x / 2.0f,
                    e->position.y + e->size.y
                };
                // bullet direction depends on enemy rotation
                Vector2 dir = e->velocity;

                BulletManager_SpawnEnemy(&game->bullets, spawnPos, dir);
            }

            // cooldown reset , with random component
            e->shootInterval = 1.0f + ((float)rand() / RAND_MAX) * 1.5f;
            e->shootCooldown = e->shootInterval;
        }
    }
}

static void Game_HandleCollisions(Game *game)
{
    // Rettangolo player
    Rectangle playerRect = {
        game->player.position.x,
        game->player.position.y,
        game->player.size.x,
        game->player.size.y
    };
    Enemy *e=NULL; // will be usefull for all enemies management

    // -- PLAYER vs ENEMY --
    for (int i = 0; i < MAX_ENEMIES; i++) {
        e = &game->enemies.enemies[i];
        if (!e->active) continue;

        Rectangle enemyRect = {
            e->position.x,
            e->position.y,
            e->size.x,
            e->size.y
        };
        //collision player and enemy
        if (CheckCollisionRecs(playerRect, enemyRect)) {

            //enemy exlode
            ExplosionManager_SpawnEnemy(
                &game->explosions,
                e->position,
                e->size
            );
            e->active = 0;
            //game->score += 10;
            PlaySound(gExplosionEnemy);

            // player invincible for invicible time (recover time) =no damage for now
            if (game->player.invincibleTime > 0.0f || !game->player.isAlive) {
                continue; 
            }

            // Damage to player
            game->energy -= 33.0f;
            if (game->energy < 0.0f) game->energy = 0.0f;

            // incincibility on but also stun (for the same time, decreased in update)
            game->player.invincibleTime = 3.0f;
            game->player.isStunned      = 1;

            // out of HP -> Explosion +  lifes manage
            if (game->energy <= 0.0f) {
                ExplosionManager_SpawnPlayer(
                    &game->explosions,
                    game->player.position,
                    game->player.size
                );
                PlaySound(gExplosionPlayer);
                Game_OnPlayerKilled(game);   //life manage 
            }
        }
    }

    // -- Bullet vs Enemy --
    Bullet *b = game->bullets.activeList;
    Bullet *prevB = NULL;

    while (b) {
        int bulletRemoved = 0;

        if (b->owner == BULLET_OWNER_PLAYER) {
            // test collection with all enemies active
            for (int i = 0; i < MAX_ENEMIES; i++) {
                Enemy *e = &game->enemies.enemies[i];
                if (!e->active) continue;

                Rectangle enemyRect = {
                    e->position.x,
                    e->position.y,
                    e->size.x,
                    e->size.y
                };

                if (CheckCollisionRecs(enemyRect, (Rectangle){
                    b->position.x,
                    b->position.y,
                    b->size.x,
                    b->size.y
                })) {
                    // enemy explode instantly
                    ExplosionManager_SpawnEnemy(
                        &game->explosions,
                        e->position,
                        e->size
                    );
                    e->active = 0;
                    game->score += 10;
                    PlaySound(gExplosionEnemy);

                    // bullet us same explosion of enemy
                    ExplosionManager_SpawnEnemy(
                        &game->explosions,
                        b->position,
                        b->size
                    );

                    //bullet remove
                    Bullet *toRemove = b;
                    b = b->next;
                    BulletManager_RemoveActive(&game->bullets, toRemove, prevB);
                    bulletRemoved = 1;
                    break;
                }
            }
        }

        if (bulletRemoved) continue;
        prevB = b;
        b = b->next;
    }

    // -- Enemy Bullet vs Player --
    prevB = NULL;
    b = game->bullets.activeList;

    while (b) {
        if (b->owner == BULLET_OWNER_ENEMY && game->player.isAlive) {
            Rectangle bulletRect = {
                b->position.x,
                b->position.y,
                b->size.x,
                b->size.y
            };
            if (CheckCollisionRecs(playerRect, bulletRect)) {
                // if player's invincibile, bullet explode but no damage
                if (game->player.invincibleTime <= 0.0f && game->player.isAlive) {
                    // gamage on player hp
                    game->energy -= b->damage;
                    if (game->energy < 0.0f) game->energy = 0.0f;

                    //set  invincibile + stun for 3 seconds (redundancy for now)
                    game->player.invincibleTime = 3.0f;
                    game->player.isStunned      = 1;

                    // out of hp -> explosion and life manage (redundancy for now)
                    if (game->energy <= 0.0f) {
                        ExplosionManager_SpawnPlayer(
                            &game->explosions,
                            game->player.position,
                            game->player.size
                        );
                        PlaySound(gExplosionPlayer);
                        Game_OnPlayerKilled(game);   
                    }
                }

                // enemy bullet explosion
                ExplosionManager_SpawnEnemy(
                    &game->explosions,
                    b->position,
                    b->size
                );

                //bullet remove
                Bullet *toRemove = b;
                b = b->next;
                BulletManager_RemoveActive(&game->bullets, toRemove, prevB);
                continue;
            }
        }

        prevB = b;
        b = b->next;
    }
//TODO better iterative fun
    // --Player Bullet vs Enemy Bullet --
    // O(N^2) for now , but MAX_BULLETS is setted on 128 not too much
    for (Bullet *b1 = game->bullets.activeList; b1; b1 = b1->next) {
        if (!b1->active) continue;

        for (Bullet *b2 = b1->next; b2; b2 = b2->next) {
            if (!b2->active) continue;

            if (b1->owner == b2->owner) continue;  // same bullet type, not collide

            Rectangle r1 = { b1->position.x, b1->position.y, b1->size.x, b1->size.y };
            Rectangle r2 = { b2->position.x, b2->position.y, b2->size.x, b2->size.y };

            if (CheckCollisionRecs(r1, r2)) {
                // both explosions
                ExplosionManager_SpawnEnemy(&game->explosions, b1->position, b1->size);
                ExplosionManager_SpawnEnemy(&game->explosions, b2->position, b2->size);

                b1->active = 0;
                b2->active = 0;
                // delegate pojectle cleaning in BulletManagerUpdate 
            }
        }
    }
}

static void Game_RespawnPlayer(Game *game)
{
    Player *p = &game->player;
    // restart position from center (more or less)
    p->position.x = (SCREEN_WIDTH  - p->size.x) / 2.0f;
    p->position.y = (PLAY_AREA_HEIGHT - p->size.y) - 20.0f;  // little highter then half

    // reset all status
    p->isAlive        = 1;
    p->isStunned      = 0;
    p->invincibleTime = 2.0f;//only when hitted 
    p->shootCooldown  = 0.0f;

    // Full HP again
    game->energy = 100.0f;
}

static void Game_OnPlayerKilled(Game *game)
{
    game->lives--;
    if (game->lives <= 0) {
        // no more lifes: game over definitivo
        game->lives = 0;
        game->player.isAlive = 0;
        game->energy = 0.0f;

        // instant gameover:
        game->currentScreen = SCREEN_GAMEOVER;
    } else {
        // respawn in one life less
        Game_RespawnPlayer(game);
    }
}