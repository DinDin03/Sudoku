#include "game.h"

Game::Game(int width, int height) : screenWidth(width), screenHeight(height), row(0), col(0) {
    InitWindow(screenWidth, screenHeight, "SUDOKU");
    SetTargetFPS(60);
    
    clearButton = { (screenWidth / 2) - 75.0f, screenHeight - 70.0f, 150.0f, 50.0f };


}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    CloseWindow();
}

void Game::Update() {
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && row > 0) row--;
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && row < GRID_SIZE - 1) row++;
    if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && col < GRID_SIZE - 1) col++;
    if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && col > 0) col--;

    for (int num = 1; num <= 9; num++) {
        if (IsKeyPressed(KEY_ONE + num - 1)) {
            grid.SetValue(row, col, num);
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, clearButton)) {
            grid.ClearGrid(); 
        }
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    grid.Draw();

    DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, {169, 169, 169, 95});

    DrawRectangleRec(clearButton, LIGHTGRAY);
    DrawText("Clear", clearButton.x + 50, clearButton.y + 15, 20, BLACK);

    EndDrawing();
}
