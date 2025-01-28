#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "raylib.h"

class Game {
private:
    int screenWidth; // Set the width of the game window
    int screenHeight; // Set the height of the game window
    Grid grid; // Create a grid
    int row, col; // declare variables for row and columns
    Rectangle clearButton;  // Create a rectangle button for clear button
    Rectangle saveButton;
    Rectangle loadButton;
    bool isStateChanged;

public:
    Game(int width, int height); // Create a game window with certain width and height 
    void Run(); // Start the game
    void Update(); // Update according to user input
    void Draw(); // Draw the grid, clear buttons etc
};

#endif
