#include "grid.h"
#include <fstream>
#include <iostream>

Grid::Grid() : grid(GRID_SIZE, std::vector<int>(GRID_SIZE, 0)) {}

void Grid::SetValue(int row, int col, int value) {
    grid[row][col] = value; // Sets the cell [row,col] to the value passed in
}

int Grid::GetValue(int row, int col){
    return grid[row][col]; // Return the value of the selected cell
}

void Grid::Draw() {
    // Draw thin lines for individual cells
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            // Draw individual cell outlines
            DrawRectangleLines(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);

            // Draw numbers in the cells
            if (grid[row][col] != 0) {
                DrawText(TextFormat("%d", grid[row][col]), col * CELL_SIZE + 35, row * CELL_SIZE + 35, 50, BLACK);
            }
        }
    }

    // Draw thicker lines for 3x3 subgrids
    for (int i = 0; i <= GRID_SIZE; i += 3) { // Iterate every 3 cells
        // Draw horizontal bold lines
        DrawLineEx(
            {0, static_cast<float>(i * CELL_SIZE)},                       // Start point
            {static_cast<float>(GRID_SIZE * CELL_SIZE), static_cast<float>(i * CELL_SIZE)}, // End point
            5.0f, // Line thickness
            BLACK // Color
        );

        // Draw vertical bold lines
        DrawLineEx(
            {static_cast<float>(i * CELL_SIZE), 0},                       // Start point
            {static_cast<float>(i * CELL_SIZE), static_cast<float>(GRID_SIZE * CELL_SIZE)}, // End point
            5.0f, // Line thickness
            BLACK // Color
        );
    }
}


void Grid::ClearGrid() {
    for (int row = 0; row < GRID_SIZE; row++) { // Iterate over each row in the grid
        for (int col = 0; col < GRID_SIZE; col++) { // Iterate over each column in the grid
            grid[row][col] = 0; // set the current cell value to 0
        }
    }
}

void Grid::SetGrid(const std::vector<std::vector<int>>& newGrid) {
    grid = newGrid;
}

bool Grid::IsValidPlacement(int row, int col, int num) const {
    // Check the row
    for (int x = 0; x < GRID_SIZE; x++) {
        if (grid[row][x] == num) return false;
    }

    // Check the column
    for (int x = 0; x < GRID_SIZE; x++) {
        if (grid[x][col] == num) return false;
    }

    // Check the 3x3 subgrid
    int startRow = (row / 3) * 3, startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if (grid[r][c] == num) return false;
        }
    }

    return true;
}

// Save the current grid to a file
bool Grid::SaveGrid(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for saving: " << filename << "\n";
        return false;
    }

    for (const auto& row : grid) {
        for (size_t col = 0; col < row.size(); col++) {
            file << row[col];
            if (col < row.size() - 1) {
                file << " "; // Separate values with a space
            }
        }
        file << "\n"; // Newline after each row
    }

    file.close();
    return true;
}

// Load a grid from a file
bool Grid::LoadGrid(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for loading: " << filename << "\n";
        return false;
    }

    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            file >> grid[row][col];
            if (file.fail()) {
                std::cerr << "Error: Invalid grid format in file: " << filename << "\n";
                return false;
            }
        }
    }

    file.close();
    return true;
}


