#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "config.h"

typedef struct UI {
    Rectangle barRect;       // grey rectangle down
    Rectangle hudRightRect;  // where will be the down-right on the bar(image 1945)
    Texture2D atlas; //for other all the images obtainable from here 
} UI;

// game data forward declaration
struct Game;

void UI_Init(UI *ui);
void UI_Draw(const UI *ui, const struct Game *game);
//void UI_Unload(UI *ui);

#endif