#include "grid.h"
#include "game.h"
#include <fstream>
#include <iostream>
#include <random>

Grid::Grid() {
    grid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    userGrid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
}

void Grid::SetValue(int row, int col, int value) {
    // Check if the value is valid before setting it
    if (grid[row][col] == 0 && IsValidPlacement(row, col, value)) { // Allow modification only if it's not a predefined number
        userGrid[row][col] = value;
    }
}


int Grid::GetValue(int row, int col) const {
    return (userGrid[row][col] != 0) ? userGrid[row][col] : grid[row][col];
}

void Grid::ClearGrid() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == 0) { // Clear only non-generated cells
                userGrid[i][j] = 0;
            }
        }
    }
}

void Grid::SetGrid(const std::vector<std::vector<int>>& newGrid) {
    grid = newGrid;
    userGrid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
}

bool Grid::IsValidPlacement(int row, int col, int num) const {
    // Check if the value is valid only if it's greater than 0
    if (num == 0) return true;

    // Check the row and column for duplicates
    for (int i = 0; i < GRID_SIZE; ++i) {
        if (GetValue(row, i) == num || GetValue(i, col) == num) {
            return false;
        }
    }
    
    // Check the 3x3 subgrid for duplicates
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (GetValue(startRow + i, startCol + j) == num) {
                return false;
            }
        }
    }
    return true;
}

void Grid::ShuffleNumbers(std::vector<int>& numbers) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);
}

void Grid::GenerateFullGrid() {
    ClearGrid();
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ShuffleNumbers(numbers);
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i][(i * 3) % GRID_SIZE] = numbers[i];
    }
    SolveSudoku();
}

void Grid::RemoveNumbers(int difficulty) {
    int attempts = 1;
    while (attempts > 0) {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;
        if (grid[row][col] != 0) {
            grid[row][col] = 0;
            attempts--;
        }
    }
}

void Grid::GenerateSudoku(int difficulty) {
    // Reset both grids
    grid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    userGrid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));

    // Generate a new puzzle
    GenerateFullGrid();
    RemoveNumbers(difficulty);
}

bool Grid::SaveGrid(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    // Save predefined grid
    file << "GRID\n";
    for (const auto& row : grid) {
        for (size_t col = 0; col < row.size(); col++) {
            file << row[col] << (col < row.size() - 1 ? " " : "\n");
        }
    }

    // Save user input grid
    file << "USERGRID\n";
    for (const auto& row : userGrid) {
        for (size_t col = 0; col < row.size(); col++) {
            file << row[col] << (col < row.size() - 1 ? " " : "\n");
        }
    }

    return true;
}

bool Grid::LoadGrid(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;

    // Read predefined grid
    std::getline(file, line); // Read "GRID"
    for (auto& row : grid) {
        for (auto& cell : row) {
            file >> cell;
            if (file.fail()) return false;
        }
    }

    // Read user input grid
    std::getline(file, line); // Read the leftover newline
    std::getline(file, line); // Read "USERGRID"
    for (auto& row : userGrid) {
        for (auto& cell : row) {
            file >> cell;
            if (file.fail()) return false;
        }
    }

    return true;
}


bool Grid::SolveSudoku() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (IsValidPlacement(row, col, num)) {
                        grid[row][col] = num;
                        if (SolveSudoku()) return true;
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void Grid::Draw() {
    // Draw thin lines for individual cells
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            // Draw individual cell outlines
            DrawRectangleLines(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);

            // Draw numbers in the cells
            int value = GetValue(row, col); // Get the value from either grid or userGrid
            if (value != 0) {
                // If the value is from userGrid, draw it in a different color (e.g., BLUE)
                if (userGrid[row][col] != 0) {
                    DrawText(TextFormat("%d", value), col * CELL_SIZE + 35, row * CELL_SIZE + 35, 50, BLUE);
                } else {
                    DrawText(TextFormat("%d", value), col * CELL_SIZE + 35, row * CELL_SIZE + 35, 50, BLACK);
                }
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

bool Grid::isSolved() const {
    // Check rows
    for (int row = 0; row < GRID_SIZE; ++row) {
        std::vector<bool> seen(GRID_SIZE, false);
        for (int col = 0; col < GRID_SIZE; ++col) {
            int value = GetValue(row, col);
            if (value == 0 || seen[value - 1]) return false;
            seen[value - 1] = true;
        }
    }

    // Check columns
    for (int col = 0; col < GRID_SIZE; ++col) {
        std::vector<bool> seen(GRID_SIZE, false);
        for (int row = 0; row < GRID_SIZE; ++row) {
            int value = GetValue(row, col);
            if (value == 0 || seen[value - 1]) return false;
            seen[value - 1] = true;
        }
    }

    // Check 3x3 subgrids
    for (int startRow = 0; startRow < GRID_SIZE; startRow += 3) {
        for (int startCol = 0; startCol < GRID_SIZE; startCol += 3) {
            std::vector<bool> seen(GRID_SIZE, false);
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 3; ++col) {
                    int value = GetValue(startRow + row, startCol + col);
                    if (value == 0 || seen[value - 1]) return false;
                    seen[value - 1] = true;
                }
            }
        }
    }

    return true;
}






