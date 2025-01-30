#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "raylib.h"

enum GameState{MENU,PLAYING};

class Game {
private:
    int screenWidth; // Set the width of the game window
    int screenHeight; // Set the height of the game window
    Grid grid; // Create a grid
    int row, col; // declare variables for row and columns
    Rectangle clearButton, saveButton, loadButton, generateButton, solveButton, easyButton, mediumButton, hardButton;  // Create a rectangle button for clear button
    bool isStateChanged;
    bool gameWon;
    double messageStartTime;
    double gameStartTime;
    double elapsedTime;
    GameState gameState;
    int difficulty;

public:
    Game(int width, int height); // Create a game window with certain width and height 
    void Run(); // Start the game
    void Update(); // Update according to user input
    void Draw(); // Draw the grid, clear buttons etc
    void DrawMenu();
    int getScreenWidth();
    int getScreenHeight();
    void invalidText(const char* text);
    void GenerateNewSudoku(int difficulty);
};

#endif
