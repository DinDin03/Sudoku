#include "grid.h"
#include "game.h"
#include <fstream>
#include <iostream>
#include <random>
using namespace std;

Grid::Grid() {
    grid = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
    userGrid = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
}

void Grid::SetValue(int row, int col, int value) {
    // If the cell is a pre-defined value, do not allow modification
    if (grid[row][col] != 0) {
        return;
    }

    // Check if the number placement is valid
    if (!IsValidPlacement(row, col, value)) {
        errorMessage = "Violates SUDOKU rules!";
        errorStartTime = GetTime(); // Store the time when the error occurred
        return;
    }

    // If valid, update the user grid
    userGrid[row][col] = value;
    errorMessage.clear(); // Clear the error message if the move is valid
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

void Grid::SetGrid(const vector<vector<int>>& newGrid) {
    grid = newGrid;
    userGrid = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
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

void Grid::ShuffleNumbers(vector<int>& numbers) {
    random_device rd;
    mt19937 g(rd());
    shuffle(numbers.begin(), numbers.end(), g);
}

void Grid::GenerateFullGrid() {
    ClearGrid();
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ShuffleNumbers(numbers);
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i][(i * 3) % GRID_SIZE] = numbers[i];
    }
    SolveSudoku();
}

void Grid::RemoveNumbers(int difficulty) {
    int attempts = difficulty;
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
    grid = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
    userGrid = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));

    // Generate a new puzzle
    GenerateFullGrid();
    RemoveNumbers(difficulty);
}

bool Grid::SaveGrid(const string& filename) const {
    ofstream file(filename);
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

bool Grid::LoadGrid(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;

    // Read predefined grid
    getline(file, line); // Read "GRID"
    for (auto& row : grid) {
        for (auto& cell : row) {
            file >> cell;
            if (file.fail()) return false;
        }
    }

    // Read user input grid
    getline(file, line); // Read the leftover newline
    getline(file, line); // Read "USERGRID"
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
            int value = GetValue(row, col);
            if (value != 0) {
                if (userGrid[row][col] != 0) {
                    DrawText(TextFormat("%d", value), col * CELL_SIZE + 35, row * CELL_SIZE + 35, 50, BLUE);
                } else {
                    DrawText(TextFormat("%d", value), col * CELL_SIZE + 35, row * CELL_SIZE + 35, 50, BLACK);
                }
            }
        }
    }

    // Draw thicker lines for 3x3 subgrids
    for (int i = 0; i <= GRID_SIZE; i += 3) { 
        DrawLineEx({0, static_cast<float>(i * CELL_SIZE)},
                   {static_cast<float>(GRID_SIZE * CELL_SIZE), static_cast<float>(i * CELL_SIZE)},
                   5.0f, BLACK);
        DrawLineEx({static_cast<float>(i * CELL_SIZE), 0},
                   {static_cast<float>(i * CELL_SIZE), static_cast<float>(GRID_SIZE * CELL_SIZE)},
                   5.0f, BLACK);
    }

    // Show error message for 2 seconds
    if (!errorMessage.empty() && GetTime() - errorStartTime <= 2.0) {
        DrawText(errorMessage.c_str(), GetScreenWidth() - 650 , GetScreenHeight() - 110, 30, RED);
    }
}


bool Grid::isSolved() const {
    // Check rows
    for (int row = 0; row < GRID_SIZE; ++row) {
        vector<bool> seen(GRID_SIZE, false);
        for (int col = 0; col < GRID_SIZE; ++col) {
            int value = GetValue(row, col);
            if (value == 0 || seen[value - 1]) return false;
            seen[value - 1] = true;
        }
    }

    // Check columns
    for (int col = 0; col < GRID_SIZE; ++col) {
        vector<bool> seen(GRID_SIZE, false);
        for (int row = 0; row < GRID_SIZE; ++row) {
            int value = GetValue(row, col);
            if (value == 0 || seen[value - 1]) return false;
            seen[value - 1] = true;
        }
    }

    // Check 3x3 subgrids
    for (int startRow = 0; startRow < GRID_SIZE; startRow += 3) {
        for (int startCol = 0; startCol < GRID_SIZE; startCol += 3) {
            vector<bool> seen(GRID_SIZE, false);
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






