#include "config.h"
#include "player.h"
#include "raylib.h"
#include "game.h"
#include "resources.h"
#include <math.h>

void PlayerInit(Player *player)
{
    player->size = (Vector2){ 32, 32 };
    // Spawn at the PLAY AREA center (not entire window)

    player->src = (Rectangle){ 4, 400, 65, 65 }; 

    player->size = (Vector2){ 65, 65 };  // dimension 1:1 of the frames from sprite sheet
    player->position = (Vector2){
        SCREEN_WIDTH / 2.0f - player->size.x / 2.0f,
        PLAY_AREA_HEIGHT / 2.0f - player->size.y / 2.0f
    };

    player->speed = 200.0f;
    player->isAlive = 1;
    player->invincibleTime = 0.0f;

    // Animation: init start values : 3 frames, one every 0.1 seconds 
    player->currentFrame = 0;
    player->frameTime    = 0.1f;
    player->frameTimer   = 0.0f;
    player->shootInterval = 0.15f; 
    player->shootCooldown = 0.0f;

    // invincibility / stun
    player->invincibleTime = 0.0f;   // no invincibility or stun at the start
    player->isStunned      = 0;  
}

// Manage basic inputs and movement of the player
void PlayerUpdate(Player *player, float dt)
{
    if (!player->isAlive) {
        // TODO: manage respawn here (?)
        return;
    }

    // --- manage invincibility / stun ---
    if (player->invincibleTime > 0.0f) {
        player->invincibleTime -= dt;
        if (player->invincibleTime <= 0.0f) {
            player->invincibleTime = 0.0f;
            player->isStunned = 0;   // stun end
        }
    }

    // if stunned, input controls blocked
    if (player->isStunned) {
        //TO DO: update animations also here (?)
        return;
    }

    Vector2 dir = { 0.0f, 0.0f };

    // INPUT (keyboard)
    if (IsKeyDown(KEY_LEFT))  dir.x -= 1.0f;
    if (IsKeyDown(KEY_RIGHT)) dir.x += 1.0f;
    if (IsKeyDown(KEY_UP))    dir.y -= 1.0f;
    if (IsKeyDown(KEY_DOWN))  dir.y += 1.0f;

    // Movement
    player->position.x += dir.x * player->speed * dt;
    player->position.y += dir.y * player->speed * dt;

    //Horizontal clamp position in PLAY AREA
    if (player->position.x < 0)
        player->position.x = 0;

    if (player->position.x + player->size.x > SCREEN_WIDTH)
        player->position.x = SCREEN_WIDTH - player->size.x;

    //Vertical clamp position in PLAY AREA
    if (player->position.y < 0)
        player->position.y = 0;

    if (player->position.y + player->size.y > PLAY_AREA_HEIGHT)
        player->position.y = PLAY_AREA_HEIGHT - player->size.y;

    // --- propeller ANIMATION ---
    player->frameTimer += dt;
    if (player->frameTimer >= player->frameTime)
    {
        player->frameTimer -= player->frameTime;
        player->currentFrame++;

        if (player->currentFrame >= 3) // abbiamo 3 frame: 0,1,2
            player->currentFrame = 0;
    }

    // TODO: Shooting part here (?)
    //TODO: Advanced position animation?
}

void PlayerDraw(const Player *player)
{
    if (!player->isAlive) return;
    
    // if invincible then blink
    if (player->invincibleTime > 0.0f) {
        // blink method found on WEB: on/off every ~0.1s
        float t = player->invincibleTime;
        if (fmodf(t, 0.2f) > 0.1f) {
            // in this interval dont drow player
            return;
        }
    }

    //  1) rect posizion and size calculation on the play area
    Rectangle dest = {
        player->position.x,
        player->position.y,
        player->size.x,
        player->size.y
    };

    // 2) calculate the frame to obtain from sprite sheet with width/3 (3 frames)
    int frameCount = 3;
    float frameWidth  = (float)gPlayerSheet.width / frameCount;
    float frameHeight = (float)gPlayerSheet.height;

    Rectangle src = {
        frameWidth * player->currentFrame, // x = 0, W, 2W
        0.0f,                              // y = 0 (is only horizontal sprite sheet)
        frameWidth,
        frameHeight
    };


    // 3)Draw the player , no offsets and 0 rotation
    DrawTexturePro(
        gPlayerSheet, //global sprite sheet from resources.h
        src,
        dest,
        (Vector2){0,0},
        0.0f,
        WHITE
    );

}
