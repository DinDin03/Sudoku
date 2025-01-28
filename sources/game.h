#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "raylib.h"

class Game {
private:
    int screenWidth;
    int screenHeight;
    Grid grid;
    int row, col;
    Rectangle clearButton; 

public:
    Game(int width, int height);
    void Run();
    void Update();
    void Draw();
};

#endif
