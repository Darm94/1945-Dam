#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct Player {
    Vector2 position;
    Vector2 size;
    float speed;
    int isAlive;
    float invincibleTime; // invicibily (and stun) time when hitted
    Rectangle src;

    // ANIMAtion
    int currentFrame;     // 0,1 or 2
    float frameTime;      // time between frames
    float frameTimer;     // time counter
    
    // Shooting
    float shootCooldown;   // bullet cooldown timer
    float shootInterval;   // max bullet cooldown value
    int isStunned;        // 1 = control blocked, 0 = normal
} Player;

void PlayerInit(Player *player);
void PlayerUpdate(Player *player, float dt);
void PlayerDraw(const Player *player);

#endif // PLAYER_H