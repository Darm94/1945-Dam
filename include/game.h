#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "config.h"
#include "player.h"
#include "ui.h"
#include "enemy.h"
#include "explosion.h"
#include "bullet.h"
#include "island.h"

//different game status
typedef enum GameScreen {
    SCREEN_MAIN_MENU,
    SCREEN_GAMEPLAY,
    SCREEN_GAMEOVER
    // TODO: add SCREEN_MENU
} GameScreen;

typedef struct Game {
    GameScreen currentScreen;

    Player player;
    EnemyManager enemies;
    BulletManager bullets;
    ExplosionManager explosions;
    IslandManager islands;

    int score;
    int lives;
    float energy;
    float waterScroll;// new scroll variable

     UI ui;

} Game;

void GameInit(Game *game);
void GameUpdate(Game *game, float dt);
void GameDraw(const Game *game);
void GameUnload(Game *game);

#endif // GAME_H