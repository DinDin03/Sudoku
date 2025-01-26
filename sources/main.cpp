#include "raylib.h"
#include <vector>
using namespace std;

#define GRID_SIZE 9
#define CELL_SIZE 100

int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    InitWindow(screenWidth, screenHeight, "SUDOKU");
    SetTargetFPS(60);
    vector<vector<int>> grid(GRID_SIZE, vector<int>(GRID_SIZE,0));
    int row = 0; int col = 0;
    
    while (!WindowShouldClose()) {

        if(IsKeyPressed(KEY_UP) && row > 0) row--;
        if(IsKeyPressed(KEY_DOWN) && row < GRID_SIZE - 1) row++;
        if(IsKeyPressed(KEY_RIGHT) && col < GRID_SIZE - 1) col++;
        if(IsKeyPressed(KEY_LEFT) && col > 0) col--;

        for (int num = 1; num <= 9; num++) {
            if (IsKeyPressed(KEY_ONE + num - 1)) {
                grid[row][col] = num;  // Update the grid with the number
            }
        }

        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        for (int row = 0; row < GRID_SIZE; row++) {
            for (int col = 0; col < GRID_SIZE; col++) {

                DrawRectangleLines(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);

                if (grid[row][col] != 0) {
                    DrawText(TextFormat("%d", grid[row][col]), col * CELL_SIZE + 15, row * CELL_SIZE + 15, 100, BLACK);
                }
            }
        }
        DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, {169, 169, 169, 95});
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
