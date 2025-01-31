#include "game.h" // import
#include "grid.h"
#include <iostream>
using namespace std;

Game::Game(int width, int height) : screenWidth(width), screenHeight(height), row(0), col(0), gameWon(false),
                                    messageStartTime(0.0), gameStartTime(GetTime()), elapsedTime(0.0),
                                    gameState(MENU), difficulty(20) { // Start in the menu

    InitWindow(screenWidth, screenHeight, "SUDOKU");
    SetTargetFPS(60);

    // Position buttons for difficulty selection
    easyButton = { static_cast<float>(screenWidth) / 2 - 150, static_cast<float>(screenHeight) / 2 - 100, 300, 60 };
    mediumButton = { static_cast<float>(screenWidth) / 2 - 150, static_cast<float>(screenHeight) / 2, 300, 60 };
    hardButton = { static_cast<float>(screenWidth) / 2 - 150, static_cast<float>(screenHeight) / 2 + 100, 300, 60 };

    // Position buttons for gameplay
    clearButton = { (screenWidth / 2) - 75.0f, screenHeight - 70.0f, 150.0f, 50.0f };
    saveButton = { clearButton.x - 175.0f, clearButton.y, 150.0f, 50.0f };
    loadButton = { clearButton.x + 175.0f, clearButton.y, 150.0f, 50.0f };
    generateButton = { clearButton.x + 350.0f, clearButton.y, 150.0f, 50.0f };
    solveButton = { clearButton.x - 350.0f, clearButton.y, 150.0f, 50.0f };
}


void Game::Run() {
    while (!WindowShouldClose()) {
        if (gameState == MENU) {
            DrawMenu();  // Show difficulty selection
        } else {
            Update();
            Draw();
        }
    }
    CloseWindow();
}

void Game::DrawMenu() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Select Difficulty", screenWidth / 2 - 120, 100, 30, DARKGRAY);

    // Draw buttons
    DrawRectangleRec(easyButton, LIGHTGRAY);
    DrawText("Easy", static_cast<int>(easyButton.x + 110), static_cast<int>(easyButton.y + 15), 30, BLACK);

    DrawRectangleRec(mediumButton, LIGHTGRAY);
    DrawText("Medium", static_cast<int>(mediumButton.x) + 90, static_cast<int>(mediumButton.y) + 15, 30, BLACK);

    DrawRectangleRec(hardButton, LIGHTGRAY);
    DrawText("Hard", static_cast<int>(hardButton.x + 110), static_cast<int>(hardButton.y + 15), 30, BLACK);

    EndDrawing();

    // Check for mouse click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, easyButton)) {
            difficulty = 20;
            GenerateNewSudoku(difficulty); // Easy = more numbers given
            gameState = PLAYING; // Switch to the game screen
        } else if (CheckCollisionPointRec(mousePos, mediumButton)) {
            difficulty = 40;
            GenerateNewSudoku(difficulty); // Medium difficulty
            gameState = PLAYING;
        } else if (CheckCollisionPointRec(mousePos, hardButton)) {
            difficulty = 60;
            GenerateNewSudoku(difficulty); // Hard = fewer numbers
            gameState = PLAYING;
        }
    }
}



double invalidInputStartTime = 0; // Store when the invalid input message starts
bool showInvalidInput = false;   // Flag to track whether to show the message
bool timeCalculated = false;

void Game::GenerateNewSudoku(int difficulty) {
    grid.GenerateSudoku(difficulty); // Generate Sudoku with specified difficulty
    gameWon = false;
    timeCalculated = false;
    gameStartTime = GetTime();
    elapsedTime = 0.0;
    messageStartTime = 0.0;
}


void Game::Update() {
    if(grid.isSolved() && !gameWon){
        gameWon = true;
        messageStartTime = GetTime();
        if(!timeCalculated){
            timeCalculated = true;
            elapsedTime = messageStartTime - gameStartTime;
        }
    }

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
            GenerateNewSudoku(difficulty);
            gameStartTime = GetTime();
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
    DrawText("Clear", static_cast<int>(clearButton.x + 50), static_cast<int>(clearButton.y + 15), 20, BLACK); // Draw a text over the button

    DrawRectangleRec(saveButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Save", static_cast<int>(saveButton.x + 50), static_cast<int>(saveButton.y + 15), 20, BLACK); // Draw a text over the button

    DrawRectangleRec(loadButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Load", static_cast<int>(loadButton.x + 50), static_cast<int>(loadButton.y + 15), 20, BLACK); // Draw a text over the button

    DrawRectangleRec(generateButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Generate", static_cast<int>(generateButton.x + 25), static_cast<int>(generateButton.y + 15), 20, BLACK); // Draw a text over the button

    DrawRectangleRec(solveButton, LIGHTGRAY); // Draw the clear button. void DrawRectangleRec(Rectangle rec, Color color);
    DrawText("Solve", static_cast<int>(solveButton.x + 50), static_cast<int>(solveButton.y + 15), 20, BLACK); // Draw a text over the button

    invalidText("Invalid Input");

    if (gameWon) {
        char message[100];
        sprintf_s(message, "Congratulations!!! Time: %.2f seconds", elapsedTime);
        DrawText(message, GetScreenWidth() - 700 , GetScreenHeight() - 110, 30, RED);
    }

    EndDrawing(); // End the drawing
}

int Game:: getScreenHeight(){
    return screenHeight;
}

int Game:: getScreenWidth(){
    return screenWidth;
}

void Game::invalidText(const char* text){
    if (showInvalidInput && GetTime() - invalidInputStartTime <= 2.0) {
        DrawText(text, screenWidth - 550 , screenHeight - 110, 30, RED);
    } else if (showInvalidInput) {
        showInvalidInput = false; // Stop showing the message after 2 seconds
    }
}

