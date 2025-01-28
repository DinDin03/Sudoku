#include "grid.h"

Grid::Grid() : grid(GRID_SIZE, std::vector<int>(GRID_SIZE, 0)) {}

void Grid::SetValue(int row, int col, int value) {
    grid[row][col] = value;
}

void Grid::Draw() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            DrawRectangleLines(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
            if (grid[row][col] != 0) {
                DrawText(TextFormat("%d", grid[row][col]), col * CELL_SIZE + 35, row * CELL_SIZE + 35, 50, BLACK);
            }
        }
    }
}

void Grid::ClearGrid() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            grid[row][col] = 0;
        }
    }
}
