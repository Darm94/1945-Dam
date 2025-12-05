#ifndef CONFIG_H
#define CONFIG_H

//i decide to try a little upper window then the original one
#define SCREEN_WIDTH   800   
#define SCREEN_HEIGHT  600   
#define UI_BAR_HEIGHT  100   // ui bar 
#define PLAY_AREA_HEIGHT (SCREEN_HEIGHT - UI_BAR_HEIGHT)

//TODO: Apply a JSON FILE CRYPTED to import the const values(?)

//---------------------------------------------------------------------------------------------
//ENEMY CONFIG CONSTANTS 

// Enemy animation
#define ENEMY_FRAME_COUNT        3        // Number of animation frames per enemy
#define ENEMY_FRAME_TIME         0.1f     // Time per animation frame

// Enemy base size (used for patterned spawn)
#define ENEMY_BASE_WIDTH         40.0f    // Default enemy width when spawning with pattern
#define ENEMY_BASE_HEIGHT        40.0f    // Default enemy height when spawning with pattern

// Enemy speeds
#define ENEMY_BASIC_SPEED        190.0f   // Default movement speed for standard enemy spawn
#define ENEMY_PATTERN_SPEED      195.0f   // Movement speed for patterned enemies

// Spawn timings for wave control
#define ENEMIES_PER_WAVE         12       // Number of enemies per wave
#define ENEMY_DELAY_BETWEEN_SPAWN 0.40f   // Delay between spawning enemies in a wave
#define ENEMY_DELAY_BETWEEN_WAVES 0.30f   // Delay between wave transitions
#define ENEMY_FIRST_SPAWN_DELAY   1.0f    // Delay before the very first wave appears

// Shooting timers
#define ENEMY_SHOOT_MIN_INTERVAL  1.0f    // Minimum enemy shooting interval
#define ENEMY_SHOOT_RANDOM_RANGE  1.5f    // Random additional time added to shoot interval
#define ENEMY_SHOOT_INITIAL_RATIO 0.5f    // Initial cooldown ratio of shoot interval

// Angle correction
#define ENEMY_ROTATION_OFFSET   -90.0f    // Apply rotation offset so sprite faces downward

// Spawn offsets
#define ENEMY_SPAWN_OUT_MIN     10.0f     // Minimum random offset outside screen spawn
#define ENEMY_SPAWN_OUT_MAX     80.0f     // Maximum random offset outside screen spawn
#define ENEMY_SIDE_EXTRA        50.0f     // Area off-screen horizontally before despawn

#endif //CONFIG_H