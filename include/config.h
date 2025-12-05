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
#define ENEMIES_PER_WAVE          12       // Number of enemies per wave
#define ENEMY_DELAY_BETWEEN_SPAWN 0.40f   // Delay between spawning enemies in a wave
#define ENEMY_DELAY_BETWEEN_WAVES 0.30f   // Delay between wave transitions
#define ENEMY_FIRST_SPAWN_DELAY   1.0f    // Delay before the very first wave appears

// Shooting timers
#define ENEMY_SHOOT_MIN_INTERVAL  1.0f    // Minimum enemy shooting interval
#define ENEMY_SHOOT_RANDOM_RANGE  1.5f    // Random additional time added to shoot interval
#define ENEMY_SHOOT_INITIAL_RATIO 0.5f    // Initial cooldown ratio of shoot interval

// Angle correction
#define ENEMY_ROTATION_OFFSET     -90.0f    // Apply rotation offset so sprite faces downward

// Spawn offsets
#define ENEMY_SPAWN_OUT_MIN       10.0f     // Minimum random offset outside screen spawn
#define ENEMY_SPAWN_OUT_MAX       80.0f     // Maximum random offset outside screen spawn
#define ENEMY_SPAWN_OFFSET        30.0f     // Another offset to add Random Result of screen spawn 
#define ENEMY_SIDE_EXTRA          50.0f     // Area off-screen horizontally before despawn

//---------------------------------------------------------------------------------------------
// SPECIFIC ENEMY PATTERN VELOCITIES & SPAWN OFFSETS

// Left → right diagonal velocity
#define ENEMY_LEFT_DIAG_VEL_X_MIN   0.4f   // Min horizontal diagonal speed (left-to-right)
#define ENEMY_LEFT_DIAG_VEL_X_MAX   0.7f   // Max horizontal diagonal speed (left-to-right)
#define ENEMY_LEFT_DIAG_VEL_Y_MIN   0.7f   // Min vertical diagonal speed
#define ENEMY_LEFT_DIAG_VEL_Y_MAX   1.0f   // Max vertical diagonal speed

// Right → left diagonal velocity
#define ENEMY_RIGHT_DIAG_VEL_X_MIN -0.7f   // Min horizontal diagonal speed (right-to-left)
#define ENEMY_RIGHT_DIAG_VEL_X_MAX -0.4f   // Max horizontal diagonal speed (right-to-left)
#define ENEMY_RIGHT_DIAG_VEL_Y_MIN  0.7f   // Min vertical diagonal speed
#define ENEMY_RIGHT_DIAG_VEL_Y_MAX  1.0f   // Max vertical diagonal speed

// Center-down slight deviation
#define ENEMY_CENTER_VEL_X_MIN     -0.2f   // Horizontal drift min for center-down
#define ENEMY_CENTER_VEL_X_MAX      0.2f   // Horizontal drift max for center-down
#define ENEMY_CENTER_VEL_Y          1.0f   // Vertical speed for center-down path

// Center horizontal spawn offset
#define ENEMY_CENTER_SPAWN_X_MIN   -60.0f  // Minimum horizontal deviation from center
#define ENEMY_CENTER_SPAWN_X_MAX    60.0f  // Maximum horizontal deviation from center

// Extra spawn shift used earlier (you already added this)
#define ENEMY_SPAWN_OFFSET          30.0f  // Extra horizontal spawn offset for diagonal patterns

//---------------------------------------------------------------------------------------------
// BULLET CONFIG CONSTANTS

// Player bullet properties
#define PLAYER_BULLET_SPEED         400.0f   // Basic player bullet speed
#define PLAYER_BULLET_DAMAGE        34.0f    // Damage dealt by player bullet
#define PLAYER_BULLET_SCALE         1.0f     // Scale applied to player bullet sprite

// Enemy bullet properties
#define ENEMY_BULLET_SPEED          250.0f   // Basic enemy bullet speed
#define ENEMY_BULLET_DAMAGE         25.0f   // Damage dealt by enemy bullet
#define ENEMY_BULLET_SCALE          0.8f     // Scale applied to enemy bullet sprite

// Bullet rendering
#define BULLET_ROTATION_OFFSET      90.0f    // Extra rotation so the bullet faces the moving direction

//---------------------------------------------------------------------------------------------
// PLAYER CONFIG CONSTANTS

// Player sprite size on screen
#define PLAYER_WIDTH                 65.0f    // Player width in pixels
#define PLAYER_HEIGHT                65.0f    // Player height in pixels

// Player source rectangle from sprite sheet
#define PLAYER_SRC_X                 4.0f     // X of player frame on sprite sheet
#define PLAYER_SRC_Y                 400.0f   // Y of player frame on sprite sheet
#define PLAYER_SRC_WIDTH             65.0f    // Width of player frame on sprite sheet
#define PLAYER_SRC_HEIGHT            65.0f    // Height of player frame on sprite sheet

// Player movement
#define PLAYER_SPEED                 220.0f   // Player movement speed

// Player animation
#define PLAYER_FRAME_COUNT           3        // Number of animation frames
#define PLAYER_FRAME_TIME            0.1f     // Time per animation frame in seconds

// Player shooting
#define PLAYER_SHOOT_INTERVAL        0.17f    // Time between player shots

// Player invincibility blink
#define PLAYER_INVINCIBLE_BLINK_PERIOD 0.2f   // Blink total period when invincible
#define PLAYER_INVINCIBLE_BLINK_ON     0.1f   // Time visible within the blink period

//--------------------------------------------------------------------------------------------
// TODO GAME CORE CONFIG *****TO APPLY

// Player stats / UI
#define GAME_INITIAL_LIVES                 3       // Number of lives at the start of the game
#define GAME_INITIAL_ENERGY                100.0f  // Player starting energy (HP bar max)

// Water background scroll
#define GAME_WATER_SCROLL_SPEED            60.0f   // Scrolling speed of the water background in pixels per second

// Enemy shooting logic
#define ENEMY_SHOOT_CHANCE                 0.5f    // Probability an enemy will shoot when cooldown ends
// Reuse already existing enemy shooting constants if present:
// #define ENEMY_SHOOT_MIN_INTERVAL       1.0f    // Minimum enemy shooting interval
// #define ENEMY_SHOOT_RANDOM_RANGE       1.5f    // Extra random time added to shoot interval

// Scoring
#define GAME_SCORE_PER_ENEMY               10      // Score gained when an enemy is destroyed

// Damage / HP changes
#define PLAYER_COLLISION_ENERGY_LOSS       33.0f   // Energy lost when colliding with an enemy
#define PLAYER_INVINCIBLE_HIT_TIME         2.0f    // Invincibility duration after being hit
#define PLAYER_INVINCIBLE_RESPAWN_TIME     2.0f    // Invincibility duration after respawn

// Player respawn position
#define PLAYER_RESPAWN_OFFSET_Y            20.0f   // Offset from bottom of play area when respawning

// Game over & UI text layout
#define GAME_OVER_TITLE_FONT_SIZE          40      // Font size for "GAME OVER" text
#define GAME_OVER_SCORE_FONT_SIZE          32      // Font size for score label on game over
#define GAME_OVER_PROMPT_FONT_SIZE         22      // Font size for "PRESS ENTER..." on game over

#define GAME_OVER_TITLE_OFFSET_X           121     // Horizontal offset from screen center for "GAME OVER"
#define GAME_OVER_TITLE_OFFSET_Y           250     // Vertical offset from screen center for "GAME OVER"

#define GAME_OVER_SCORE_OFFSET_X           96      // Horizontal offset from screen center for score text
#define GAME_OVER_SCORE_OFFSET_Y           200     // Vertical offset from screen center for score text

#define GAME_OVER_PROMPT_OFFSET_Y          80      // Offset from bottom for "PRESS ENTER TO RETURN TO MENU"

// Main menu text layout
#define MENU_TITLE_FONT_SIZE               28      // Font size for main title in menu
#define MENU_PROMPT_FONT_SIZE              24      // Font size for "PRESS ENTER TO START" in menu

#define MENU_TITLE_OFFSET_X                232     // Horizontal offset from screen center for menu title
#define MENU_TITLE_OFFSET_Y                10      // Vertical offset from screen center for menu title

#define MENU_PROMPT_OFFSET_X               145     // Horizontal offset from screen center for menu prompt
#define MENU_PROMPT_OFFSET_Y               180     // Vertical offset from screen center for menu prompt

#endif //CONFIG_H