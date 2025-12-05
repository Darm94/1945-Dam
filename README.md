# 1945Demo (Strikers 1945 - Damiano Edition)

A classic arcade-style vertical shoot 'em up (shmup) developed in **C** using the **raylib** library.

## Build and Setup

This project uses **CMake** for build management.

### Prerequisites

1.  **raylib:** You must have the **raylib** library (version 4.5 or higher recommended) downloaded.
    * Download the precompiled version for your OS and place the necessary files (e.g., `raylib.dll`, `libraylib.a`) into the project's `lib/` folder, as indicated in the structure below.
2.  **C Compiler and CMake:** Ensure you have a C development environment (like MinGW, GCC/Clang) and **CMake** installed.

### Project Structure

The project expects the following file structure for CMake to function correctly:
```
1945Demo/
├── src/
│   ├── main.c
│   ├── game.c        <-- Il file principale del gioco
│   ├── ... (altri .c)
├── include/
│   ├── config.h
│   ├── game.h
│   ├── ... (altri .h)
├── resources/        <-- Tutte le immagini, suoni, texture
│   ├── water.png
│   ├── Icon.ico
│   ├── ...
├── lib/
│   ├── raylib.dll    <-- Da raylib/bin (Windows)
│   ├── libraylib.a   <-- Da raylib/lib
├── CMakeLists.txt
└── README.md
```

### Compilation Instructions (CMake)

Execute the following terminal commands from the project's root directory (`1945Demo/`):

1.  **Create the build directory:**
    ```bash
    mkdir build
    cd build
    ```

2.  **Generate build files:**
    ```bash
    cmake ..
    ```
    * *Note for Visual Studio:* If you are using the CMake plugin for Visual Studio, simply open the root project folder, and Visual Studio will handle this configuration step automatically.

3.  **Compile the project:**
    ```bash
    cmake --build .
    ```

#### Compilation Result

The main executable, named **`1945_damiano`**, will be generated in the **`bin/`** directory.
The `CMakeLists.txt` is configured to automatically copy the `raylib.dll` and the entire `resources/` folder into `bin/` to ensure the executable runs immediately.

---

## Game Code Structure (`game.c`)

The `game.c` file implements the core game loop and manages the overall application state through the `Game` structure.

### 1. Game Screens

The game is divided into three states managed by an `enum` (`SCREEN_GAMEPLAY`, `SCREEN_MAIN_MENU`, `SCREEN_GAMEOVER`):

| State | Description | Transition |
| :--- | :--- | :--- |
| **SCREEN_MAIN_MENU** | Initial splash/start screen. | **ENTER** $\rightarrow$ Gameplay. |
| **SCREEN_GAMEPLAY** | Active game state. | Zero Lives $\rightarrow$ Game Over. |
| **SCREEN_GAMEOVER** | Final score screen. | **ENTER** $\rightarrow$ Main Menu (and game reset). |

### 2. Main Game Loop Functions

The code follows the standard raylib Game Loop pattern:

* **`GameInit(Game *game)`**: Initializes all state variables (`score`, `lives`, `energy`, etc.) and initializes all entity *Managers* (Player, EnemyManager, BulletManager, etc.). Called at startup and when resetting the game.
* **`GameUpdate(Game *game, float dt)`**: Handles game logic based on the current state:
    * Updates Player, Enemies, Bullets, and Explosions.
    * Manages the **background water scroll** (`waterScroll`).
    * Handles shooting input (`Game_HandlePlayerShooting`) and enemy firing logic (`Game_HandleEnemyShooting`).
    * Manages **collisions** (`Game_HandleCollisions`).
* **`GameDraw(const Game *game)`**: Draws all game elements in the correct visual order: Water Background $\rightarrow$ Islands $\rightarrow$ Enemies $\rightarrow$ Bullets $\rightarrow$ Player $\rightarrow$ Explosions $\rightarrow$ UI.

### 3. Collision Logic (`Game_HandleCollisions`)

This function implements the critical interactions between entities:

1.  **Player vs Enemy:** Fatal collision for the enemy, damage dealt to the player, and activation of invincibility/stun.
2.  **Player Bullet vs Enemy:** Destroys the enemy and the bullet, increments the **score**.
3.  **Enemy Bullet vs Player:** Deals damage to the player (if not invincible), destroys the bullet.
4.  **Bullet vs Bullet:** Collision between player and enemy projectiles (both are destroyed).

#### Life and Respawn Management

The player's health system is managed by `game->energy` (HP) and `game->lives` (extra lives).

* If `game->energy` reaches $\le 0.0\text{f}$, **`Game_OnPlayerKilled`** is called.
* `Game_OnPlayerKilled` decreases `lives`. If `lives > 0`, the plane is resurrected via **`Game_RespawnPlayer`** (which restores energy and position). If `lives = 0`, the state is switched to `SCREEN_GAMEOVER`.
