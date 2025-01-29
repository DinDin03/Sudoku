#include "game.h"
#include <iostream>
using namespace std;

Game::Game(int width, int height) : screenWidth(width), screenHeight(height), row(0), col(0) {
    InitWindow(screenWidth, screenHeight, "SUDOKU"); // Initialise the window with given screenwidth, screenHeight and call it SUDOKU. InitWindow(int width, int height, const char *title);
    SetTargetFPS(60); // Set the FPS to 60
    
    clearButton = { (screenWidth / 2) - 75.0f, screenHeight - 70.0f, 150.0f, 50.0f }; // Creates a button for the clear button. Rectangle(float x, float y, float width, float height) in pixels
    saveButton = { clearButton.x - 175.0f, clearButton.y, 150.0f, 50.0f };
    loadButton = { clearButton.x + 175.0f, clearButton.y, 150.0f, 50.0f };
    generateButton = { clearButton.x + 350.0f, clearButton.y, 150.0f, 50.0f };
    solveButton = { clearButton.x - 350.0f, clearButton.y, 150.0f, 50.0f };

}

void Game::Run() {
    while (!WindowShouldClose()) { // While the window is open,
        Update(); // keep updating the grid
        Draw(); // Keep drawing
    }
    CloseWindow(); // Otherwise close the window
}

double invalidInputStartTime = 0; // Store when the invalid input message starts
bool showInvalidInput = false;   // Flag to track whether to show the message

void Game::Update() {
    int key = GetKeyPressed(); // Capture the key pressed
    if (IsKeyPressed(KEY_UP) && row > 0) row--; // If the up button or "W" is pressed, change row position by -y
    else if (IsKeyPressed(KEY_DOWN) && row < GRID_SIZE - 1) row++; // If the down button or "S" is pressed, change the row position by +y
    else if (IsKeyPressed(KEY_RIGHT) && col < GRID_SIZE - 1) col++; // If the right button or "D" is pressed, change the col by +x
    else if (IsKeyPressed(KEY_LEFT) && col > 0) col--; // if the left button or "A" is pressed, change the col by -x
    else if(IsKeyPressed(KEY_BACKSPACE) && grid.GetValue(row,col) != 0) grid.SetValue(row,col,0); // If the backspace key is set the current value of the cell to 0
    else{
        if ((key < 49 || key > 57) && key != 0) { // Check if it's not a number and not 0
        invalidInputStartTime = GetTime();
        showInvalidInput = true;
    }
    }

    for (int num = 1; num <= 9; num++) {
        if (IsKeyPressed(KEY_ONE + num - 1)) { // If any number key is pressed 
            grid.SetValue(row, col, num); // Set the value to that number
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { // If mouse left click is pressed
        Vector2 mousePos = GetMousePosition(); // Create a vector which stores 2 values (x,y) and get the mouse position
        if (CheckCollisionPointRec(mousePos, clearButton)) { // If mousePos is within the range of clearButton,
            grid.ClearGrid(); // Clear the grid
        }
        else if(CheckCollisionPointRec(mousePos, saveButton)){
            grid.SaveGrid("grid.txt");
        }
        else if(CheckCollisionPointRec(mousePos, loadButton)){
            grid.LoadGrid("grid.txt");
        }
        else if(CheckCollisionPointRec(mousePos, generateButton)){
            grid.GenerateSudoku(5);
        }
        else if(CheckCollisionPointRec(mousePos, solveButton)){
            grid.SolveSudoku();
        }
        else{
            col = static_cast<int>(round(mousePos.x)) / CELL_SIZE;
            row = static_cast<int>(round(mousePos.y)) / CELL_SIZE;
        }
    }
}

void Game::Draw() {
    BeginDrawing(); // Begin drawing, set up window
    ClearBackground(RAYWHITE); // Clear the background by setting it to ray white

    grid.Draw(); // Draw the grid

    DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, {169, 169, 169, 95}); // Draw rectangles for cells. void DrawRectangle(int posX, int posY, int width, int height, Color color);

    DrawRectangleRec(clearButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Clear", clearButton.x + 50, clearButton.y + 15, 20, BLACK); // Draw a text over the button

    DrawRectangleRec(saveButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Save", saveButton.x + 50, saveButton.y + 15, 20, BLACK); // Draw a text over the button

    DrawRectangleRec(loadButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Load", loadButton.x + 50, loadButton.y + 15, 20, BLACK); // Draw a text over the button

    DrawRectangleRec(generateButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Generate", generateButton.x + 25, generateButton.y + 15, 20, BLACK); // Draw a text over the button

    DrawRectangleRec(solveButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Solve", solveButton.x + 50, solveButton.y + 15, 20, BLACK); // Draw a text over the button

    if (showInvalidInput && GetTime() - invalidInputStartTime <= 1.0) {
        DrawText("Invalid Input", screenWidth - 200, screenHeight - 50, 20, RED);
    } else if (showInvalidInput) {
        showInvalidInput = false; // Stop showing the message after 2 seconds
    }

    EndDrawing(); // End the drawing
}
